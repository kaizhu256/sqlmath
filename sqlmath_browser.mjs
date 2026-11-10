/*jslint beta, bitwise, browser, devel, nomen*/
import {
    assertOrThrow,
    csvFromListofList,
    dbCloseAsync,
    dbExecAsync,
    dbFileSaveAsync,
    dbFileLoadAsync,
    dbOpenAsync,
    dbTableImportAsync,
    debugInline,
    noop,
    sqlmathWebworkerInit
} from "./sqlmath.mjs";
let CRISPX = -0.5;
let CRISPY = 0.5;
let {
    CodeMirror
} = window;
let DBTABLE_DICT = new Map();
let DB_CHART;
let DB_DICT = new Map();
let DB_EXEC_CHAIN = Promise.resolve();
let DB_INIT = Promise.resolve();
let DB_MAIN;
let DB_QUERY;
let DEBOUNCE_DICT = Object.create(null);
let UI_ANIMATE_DATENOW = Date.now();
let UI_ANIMATE_DURATION = 250;
let UI_ANIMATE_DURATION_INV = 1 / UI_ANIMATE_DURATION;
let UI_ANIMATE_LIST = [];
let UI_CONTEXTMENU = document.getElementById("contextmenu1");
let UI_CONTEXTMENU_BATON;
let UI_CRUD = document.getElementById("crudPanel1");
let UI_CRUD_BATON;
let UI_EDITOR;
let UI_FILE_OPEN = document.createElement("input");
let UI_FILE_SAVE = document.createElement("a");
let UI_LOADING = document.getElementById("loadingPanel1");
let UI_LOADING_COUNTER = 0;
let UI_LONGPRESS_FIRED;
let UI_LONGPRESS_TIMER;
let UI_LONGPRESS_XY = [
    0, 0
];
let UI_PAGE_SIZE = 256;
let UI_RESIZE_WIDTH = window.innerWidth;
let UI_ROW_HEIGHT = 16;
let UI_VIEW_SIZE = 20;

noop(debugInline);

async function dbCloseWhenIdle(db) {
// this function will close <db> once its pending queries finish, since
// dbCloseAsync throws while db.busy > 0, which leaked the wasm handle
    while (db.busy > 0) {
        await waitAsync(100);
    }
    await dbCloseAsync(db);
}

// REVIEWED r18: a user ATTACH named attach01 blocks it until detached
async function dbFileAttachAsync({
    db,
    dbData
}) {
// this function will attach database <dbData> to <db>
    let dbAttached;
    let dbName = dbNameNext("attach{{ii}}", new Set(DB_DICT.keys()));
    let isAttached;
    dbAttached = await dbOpenAsync({
        filename: `file:${dbName}?mode=memory&cache=shared`
    });
    dbAttached.dbName = dbName;
    try {
        await dbFileLoadAsync({
            db: dbAttached,
            dbData
        });
        await dbExecAsync({
            db,
            sql: `ATTACH DATABASE '${dbAttached.filename}' AS ${dbName}`
        });
        isAttached = true;
    } finally {
        // failed load or ATTACH - close handle, else it leaks in wasm
        if (!isAttached) {
            await dbCloseAsync(dbAttached);
        }
    }
    DB_DICT.set(dbName, dbAttached);
    // normalize order - REVIEWED r17: a one-Map sort is churn
    DB_DICT = Array.from(DB_DICT.values());
    DB_DICT = new Map([
        DB_DICT.slice(0, 3),
        DB_DICT.slice(3).sort(function (aa, bb) {
            aa = aa.dbName;
            bb = bb.dbName;
            return (
                aa < bb
                ? -1
                : 1
            );
        })
    ].flat().map(function (db) {
        return [
            db.dbName, db
        ];
    }));
}

function dbNameNext(template, bag) {
// this function will get next incremental name in <bag> from given <template>
    let ii = 0;
    let name;
    while (true) {
        ii += 1;
        name = template.replace("{{ii}}", String(ii).padStart(2, "0"));
        if (!bag.has(name)) {
            return name;
        }
    }
}

function debounce(key, func, ...argList) {
// this function will debounce <func> with given <key>
    let val = DEBOUNCE_DICT[key];
    if (val) {
        val.argList = argList;
        val.func = func;
        return;
    }
    val = {
        argList,
        func: noop,
        timerTimeout: setTimeout(function () {
            delete DEBOUNCE_DICT[key];
            val.func(...val.argList);
        }, 250)
    };
    DEBOUNCE_DICT[key] = val;
    // if first-time, then immediately call <func>
    func(...argList);
}

async function demoDefault() {
// this function will run demo-default
    // attach demo-db
    await dbFileAttachAsync({
        db: DB_MAIN,
        dbData: new ArrayBuffer(0)
    });
    UI_EDITOR.setValue(String(`
DROP TABLE IF EXISTS __stock_historical;
CREATE TABLE __stock_historical(sym TEXT, date TEXT, price REAL);
INSERT INTO __stock_historical (sym, date, price) VALUES
    ('aapl', '2020-01-01', 77.37),  ('aapl', '2020-02-01', 68.33),
    ('aapl', '2020-03-01', 63.57),  ('aapl', '2020-04-01', 73.44),
    ('aapl', '2020-05-01', 79.48),  ('aapl', '2020-06-01', 91.19),
    ('aapl', '2020-07-01', 106.26),  ('aapl', '2020-08-01', 129.03),
    ('aapl', '2020-09-01', 115.80),  ('aapl', '2020-10-01', 108.86),
    ('aapl', '2020-11-01', 119.05),  ('aapl', '2020-12-01', 132.69),
    ('goog', '2020-01-01', 1434.23),  ('goog', '2020-02-01', 1339.33),
    ('goog', '2020-03-01', 1162.81),  ('goog', '2020-04-01', 1348.66),
    ('goog', '2020-05-01', 1428.92),  ('goog', '2020-06-01', 1413.61),
    ('goog', '2020-07-01', 1482.96),  ('goog', '2020-08-01', 1634.18),
    ('goog', '2020-09-01', 1469.60),  ('goog', '2020-10-01', 1621.01),
    ('goog', '2020-11-01', 1760.74),  ('goog', '2020-12-01', 1751.88);

DROP TABLE IF EXISTS __test1;
CREATE TABLE __test1(
    col1,
    col2,
    column_long_name_aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
);

DROP TABLE IF EXISTS __test2;
CREATE TABLE __test2 AS
    SELECT * FROM __test1
    UNION ALL SELECT 1, 2, 3
    UNION ALL SELECT 4, 5, 6;

DROP TABLE IF EXISTS attach01.__test3;
CREATE TABLE attach01.__test3 AS SELECT * FROM __test2;

SELECT
        *,
        random() AS c1,
        random() AS c2,
        random() AS c3,
        random() AS c4,
        random(),
        random(),
        random(),
        random(),
        1 AS sentinel
    FROM __stock_historical
    LEFT JOIN __test1 ON __test1.col1 = __stock_historical.sym
    CROSS JOIN (SELECT random() FROM __stock_historical);

DROP TABLE IF EXISTS chart.__stock_chart;
CREATE TABLE chart.__stock_chart (
    datatype TEXT NOT NULL,
    series_index INTEGER,
    xx REAL,
    yy REAL,
    series_label REAL,
    xx_label TEXT,
    options TEXT
);
INSERT INTO chart.__stock_chart (datatype, options)
    SELECT
        'options' AS datatype,
        '{
            "title": "price vs. date comparison of multiple stocks",
            "xaxisTitle": "date",
            "yaxisTitle": "percent gain",
            "yvalueSuffix": " %"
        }' AS options;
INSERT INTO chart.__stock_chart (datatype, series_index, series_label)
    SELECT
        'series_label' AS datatype,
        rownum AS series_index,
        sym AS series_label
    FROM (
        SELECT
            ROW_NUMBER() OVER (ORDER BY sym) AS rownum,
            sym
        FROM (SELECT DISTINCT sym FROM __stock_historical)
        WHERE
            sym IS NOT NULL
    );
INSERT INTO chart.__stock_chart (datatype, xx, xx_label)
    SELECT
        'xx_label' AS datatype,
        rownum AS xx,
        date AS xx_label
    FROM (
        SELECT
            ROW_NUMBER() OVER (ORDER BY date) AS rownum,
            date
        FROM (SELECT DISTINCT date FROM __stock_historical)
    );
INSERT INTO chart.__stock_chart (datatype, series_index, xx, yy)
    SELECT
        'yy_value' AS datatype,
        series_index,
        xx,
        price AS yy
    FROM (
        SELECT
            series_index,
            series_label,
            xx,
            xx_label
        FROM (
            SELECT
                series_index,
                series_label
            FROM chart.__stock_chart
            WHERE
                datatype = 'series_label'
        )
        JOIN (
            SELECT
                xx,
                xx_label
            FROM chart.__stock_chart
            WHERE
                datatype = 'xx_label'
        )
    )
    LEFT JOIN __stock_historical ON sym = series_label AND date = xx_label;
UPDATE chart.__stock_chart
    SET
        yy = yy * inv - 1
    FROM (
    --
    SELECT
        1.0 / yy AS inv,
        series_index
    FROM (
        SELECT
            ROW_NUMBER() OVER (
                PARTITION BY series_index ORDER BY xx
            ) AS rownum,
            yy,
            series_index
        FROM chart.__stock_chart
        WHERE
            datatype = 'yy_value'
            AND yy > 0
    )
    WHERE
        rownum = 1
    --
    ) AS __join1 WHERE __join1.series_index = __stock_chart.series_index;
    `).trim() + "\n");
    await onDbExec({});
    return true;
}

async function demoTradebot() {
// this function will run demo-tradebot
    let tmp;
    let tradebotState;
    // one chart ddl for every demo chart below
    function sqlChartCreate(tableChart) {
        return String(`
DROP TABLE IF EXISTS ${tableChart};
CREATE TABLE ${tableChart} (
    datatype TEXT NOT NULL,
    series_index INTEGER,
    xx REAL,
    yy REAL,
    series_label REAL,
    xx_label TEXT,
    options TEXT
);
        `).trim();
    }
    // any failure up to tradebot_state returns falsy - init falls back to
    // demoDefault; REVIEWED: a truncated file shows "malformed", rare
    try {
        tmp = await fetch(`.tradebot_public.sqlite?aa=${Date.now()}`);
        if (tmp.status !== 200) {
            return;
        }
        await dbFileLoadAsync({
            db: DB_MAIN,
            dbData: await tmp.arrayBuffer()
        });
        tradebotState = noop(
            await dbExecAsync({
                db: DB_MAIN,
                sql: "SELECT * FROM tradebot_state;"
            })
        )[0][0];
    } catch (ignore) {
        return;
    }
    if (!tradebotState) {
        return;
    }
    // datenow is "YYYY-MM-DD HH:MM:SS"; safari cannot Date-parse the space,
    // only the iso "T" form
    tradebotState.datenowUtc = new Date(
        String(tradebotState.datenow).replace(" ", "T") + "Z"
    ).toUTCString();
    UI_EDITOR.setValue([
        (`
-- table - tradebot_intraday_day - insert
DROP TABLE IF EXISTS tradebot_intraday_day;
CREATE TABLE tradebot_intraday_day AS
    SELECT
        sym,
        xdate,
        price
    FROM tradebot_intraday
    WHERE xdate >= (SELECT datemkt0_beg FROM tradebot_state)
    --
    UNION ALL
    --
    SELECT
        sym,
        DATETIME(datemkt0_beg, '-1 MINUTE') AS xdate,
        price
    FROM tradebot_historical
    JOIN tradebot_state
    WHERE tradebot_historical.xdate = datemkt0_lag;

-- table - tradebot_intraday_month - insert
DROP TABLE IF EXISTS tradebot_intraday_month;
CREATE TABLE tradebot_intraday_month AS
    SELECT
        sym,
        xdate,
        price
    FROM tradebot_intraday
    WHERE xdate = xdate2;

-- table - tradebot_tech_intra_day - insert - lmt
DROP TABLE IF EXISTS tradebot_tech_intra_day;
CREATE TABLE tradebot_tech_intra_day(
    tname TEXT,
    xdate REAL,
    xdate2 REAL,
    tval REAL
);
INSERT INTO tradebot_tech_intra_day
    SELECT
        *
    FROM (
        SELECT
                '1a_stk_pct' AS tname,
                xdate,
                xdate2,
                stk_pct AS tval
            FROM tradebot_tech_intra
        --
        UNION ALL
        --
        SELECT '1b_stk_lmt', xdate, xdate2, stk_lmt FROM tradebot_tech_intra
        --
        UNION ALL
        --
        SELECT '1c_stk_pnl', xdate, xdate2, stk_pnl FROM tradebot_tech_intra
        --
        UNION ALL
        --
        SELECT '2a_spy_prc', xdate, xdate2, spy_prc FROM tradebot_tech_intra
        --
        UNION ALL
        --
        SELECT '2b_spy_cos', xdate, xdate2, spy_cos FROM tradebot_tech_intra
        --
        UNION ALL
        --
        SELECT '2c_spy_sin', xdate, xdate2, spy_sin FROM tradebot_tech_intra
        --
        UNION ALL
        --
        SELECT '2d_spy_syy', xdate, xdate2, spy_syy FROM tradebot_tech_intra
        --
        UNION ALL
        --
        SELECT '2e_spy_zcl', xdate, xdate2, spy_zcl FROM tradebot_tech_intra
    )
    WHERE xdate AND tval IS NOT NULL;

-- table - tradebot_tech_intra_month - insert - lmt
DROP TABLE IF EXISTS tradebot_tech_intra_month;
CREATE TABLE tradebot_tech_intra_month(
    tname TEXT,
    xdate REAL,
    xdate2 REAL,
    tval REAL
);
INSERT INTO tradebot_tech_intra_month
    SELECT * FROM tradebot_tech_intra_day WHERE xdate = xdate2;
DELETE FROM tradebot_tech_intra_day
    WHERE xdate < (SELECT datemkt0 FROM tradebot_state);
ALTER TABLE tradebot_tech_intra_day DROP COLUMN xdate2;
ALTER TABLE tradebot_tech_intra_month DROP COLUMN xdate2;
        `),
        [
            "1 day",
            "1 month",
            "ytd",
            "1 year",
            "3 year",
            "3 year reverse timeline"
        ].map(function (dateInterval) {
            let optionDict;
            let tableChart;
            let tableData;
            tableData = (
                dateInterval === "1 day"
                ? "tradebot_intraday_day"
                : dateInterval === "1 month"
                ? "tradebot_intraday_month"
                : "tradebot_historical"
            );
            tableChart = (
                "chart._{{ii}}_tradebot_historical_" +
                dateInterval.replace((
                    /\W/g
                ), "_")
            );
            optionDict = {
                title: (
                    "tradebot historical performance vs market - " +
                    dateInterval +
                    (
                        dateInterval === "1 day"
                        ? "\n[ updated " + tradebotState.datenowUtc + " ]"
                        : ""
                    )
                ),
                xaxisTitle: "date",
                xstep: (
                    dateInterval === "1 day"
                    ? 60
                    : dateInterval === "1 month"
                    ? 15 * 60
                    : 1
                ),
                xvalueConvert: (
                    (dateInterval === "1 day" || dateInterval === "1 month")
                    ? "unixepochToTimeutc"
                    : "juliandayToDate"
                ),
                yaxisTitle: "percent gain",
                yvalueSuffix: " %"
            };
            return (`
-- chart - ${tableChart} - create
${sqlChartCreate(tableChart)}
INSERT INTO ${tableChart} (datatype, options)
    SELECT
        'options' AS datatype,
        '${JSON.stringify(optionDict)}' AS options;
INSERT INTO ${tableChart} (datatype, options, series_index, series_label)
    SELECT
        'series_label' AS datatype,
        JSON_OBJECT(
            'isDummy', is_dummy,
            'isHidden', NOT sym IN ('11_mybot', 'spy')
        ) AS options,
        rownum AS series_index,
        sym AS series_label
    FROM (
        SELECT
            sym LIKE '-%' AS is_dummy,
            ROW_NUMBER() OVER (
                ORDER BY
                    sym = '11_mybot' DESC,
                    sym = '----' DESC,
                    sym = 'spy' DESC,
                    sym = 'qqq' DESC,
                    sym = 'dia' DESC,
                    sym = '---- ' DESC,
                    sym
            ) AS rownum,
            sym
        FROM (
            SELECT DISTINCT sym FROM ${tableData}
            --
            UNION ALL
            --
            SELECT '----'
            --
            UNION ALL
            --
            SELECT '---- '
        )
    );
DROP TABLE IF EXISTS __tmp1;
CREATE TEMP TABLE __tmp1 AS
    SELECT
        *
    FROM (SELECT DISTINCT xdate FROM ${tableData})
    JOIN (SELECT MIN(xdate) AS aa, MAX(xdate) AS bb FROM ${tableData});
UPDATE __tmp1
    SET
        aa = aa2
    FROM (
        SELECT
            xdate AS aa2
        FROM __tmp1
        JOIN (
            SELECT
                MAX(aa,
            ${
                (
                    dateInterval === "3 year reverse timeline"
                    ? "DATE(bb, '-3 YEAR')"
                    : dateInterval === "ytd"
                    ? "DATE(STRFTIME('%Y', bb) || '-01-01', '-1 DAY')"
                    : "DATE(bb, '-" + dateInterval + "')"
                )
            }
                ) AS aa2
            FROM (SELECT aa, bb FROM __tmp1 LIMIT 1)
        )
        WHERE
            xdate <= aa2
        ORDER BY
            xdate DESC
        LIMIT 1
    );
INSERT INTO ${tableChart} (datatype, xx, xx_label)
    SELECT
        'xx_label' AS datatype,
        rownum AS xx,
        xdate AS xx_label
    FROM (
        SELECT
            ROW_NUMBER() OVER (ORDER BY xdate ASC) AS rownum,
            xdate
        FROM __tmp1
        WHERE
            aa <= xdate AND xdate <= bb
    );
INSERT INTO ${tableChart} (datatype, series_index, xx, yy)
    SELECT
        'yy_value' AS datatype,
        series_index,
        xx,
        price AS yy
    FROM (
        SELECT
            series_index,
            series_label,
            xx,
            xx_label
        FROM (
            SELECT
                series_index,
                series_label
            FROM ${tableChart}
            WHERE
                datatype = 'series_label'
        )
        JOIN (
            SELECT
                xx,
                xx_label
            FROM ${tableChart}
            WHERE
                datatype = 'xx_label'
        )
    )
    LEFT JOIN ${tableData} ON sym = series_label AND xdate = xx_label;
UPDATE ${tableChart}
    SET
            ${
                (
                    dateInterval === "3 year reverse timeline"
                    ? "yy = ROUND(100 * (1.0 / (yy * inv) - 1), 4)"
                    : "yy = ROUND(100 * (yy * inv - 1), 4)"
                )
            }
    FROM (
    --
    SELECT
        1.0 / yy AS inv,
        series_index
    FROM (
        SELECT
            ROW_NUMBER() OVER (
                PARTITION BY series_index ORDER BY xx
            ${
                (
                    dateInterval === "3 year reverse timeline"
                    ? "DESC"
                    : "ASC"
                )
            }
            ) AS rownum,
            yy,
            series_index
        FROM ${tableChart}
        WHERE
            datatype = 'yy_value'
            AND yy > 0
    )
    WHERE
        rownum = 1
    --
    ) AS __join1 WHERE __join1.series_index = ${tableChart}.series_index;
UPDATE ${tableChart}
    SET
        series_label = printf(
            '%+06.2f%% - %s%s',
            yy_today,
            series_label,
            IIF(CASTTEXTOREMPTY(company_name) = '', '', ' - ' || company_name)
        )
    FROM (
    --
    SELECT
        ${tableChart}.rowid,
        --
        company_name,
        yy_today
    FROM ${tableChart}
    --
    LEFT JOIN (
        SELECT
            series_index,
            yy_today
        FROM (
            SELECT
                ROW_NUMBER() OVER (
                    PARTITION BY series_index ORDER BY xx DESC
                ) AS rownum,
                series_index,
                yy AS yy_today
            FROM ${tableChart}
            WHERE
                datatype = 'yy_value'
        )
        WHERE
            rownum = 1
    ) USING (series_index)
    LEFT JOIN tradebot_stock_basket ON sym = series_label
    WHERE
        datatype = 'series_label'
    --
    ) AS __join1 WHERE __join1.rowid = ${tableChart}.rowid;
-- chart - tradebot_historical - normalize xx to unixepoch
UPDATE ${tableChart}
    SET
        xx = ${(
                (dateInterval === "1 day" || dateInterval === "1 month")
                ? "UNIXEPOCH(xdate)"
                : "JULIANDAY(xdate)"
            )}
    FROM (
    --
    SELECT
        ${tableChart}.rowid,
        --
        xdate
    FROM ${tableChart}
    --
    JOIN (
        SELECT
            xx,
            xx_label AS xdate
        FROM ${tableChart}
        WHERE
            datatype = 'xx_label'
    ) USING (xx)
    WHERE
        datatype = 'yy_value'
    --
    ) AS __join1 WHERE __join1.rowid = ${tableChart}.rowid;
INSERT INTO ${tableChart} (datatype, series_index, xx, yy)
    SELECT
        'yy_value' AS datatype,
        series_index,
        xx0 AS xx,
        0 AS yy
    FROM (
        SELECT
            series_index
        FROM ${tableChart}
        WHERE
            datatype = 'series_label'
    )
    JOIN (
        SELECT
            MIN(xx) AS xx0
        FROM ${tableChart}
        WHERE
            datatype = 'yy_value'
    )
    WHERE
        ${(dateInterval === "1 day" || dateInterval === "1 month")};
DELETE FROM ${tableChart} WHERE datatype = 'xx_label';
            `);
        }),
        [
            "subindustry",
            "stock"
        ].map(function (grouping) {
            return [
                "performance",
                "holding"
            ].map(function (ptype) {
                let columnData;
                let optionDict;
                let sqlSelect;
                let tableChart;
                columnData = (
                    ptype === "performance"
                    ? "perc_gain_today"
                    : "perc_holding"
                );
                optionDict = {
                    isBarchart: true,
                    title: `tradebot ${ptype} today by ${grouping}`,
                    xaxisTitle: grouping,
                    yaxisTitle: (
                        ptype === "performance"
                        ? "percent gain"
                        : "percent holding"
                    ),
                    yvalueSuffix: " %"
                };
                sqlSelect = (
                    grouping === "subindustry"
                    ? (`
SELECT
        (CASE
            WHEN (category LIKE 'index%') THEN 3
            WHEN (category LIKE '___-') THEN 1
            ELSE 5
        END) AS series_color,
        category LIKE '-%' AS is_dummy,
        0 AS is_hidden,
        printf('%05.4f%% - %s', ${columnData}, category) AS series_label,
        ROW_NUMBER() OVER (
            ORDER BY
                grouping_index,
                category != '----' DESC,
                ${columnData} DESC
        ) AS xx,
        category AS xx_label,
        ${columnData} AS yy
    FROM (
        SELECT
            category,
            grouping,
            grouping_index,
            ${columnData},
            perc_holding
        FROM tradebot_position_category
        WHERE
            grouping = 'subindustry'
    )
                    `)
                    : (`
SELECT
        (CASE
            WHEN (subindustry LIKE '___+') THEN 3
            WHEN (subindustry LIKE '___-') THEN 1
            ELSE 2
        END) AS series_color,
        0 AS is_dummy,
        0 AS is_hidden,
        printf(
            '%+.4f%% - %s - %s - %s',
            ${columnData},
            'long',
            sym,
            company_name
        ) AS series_label,
        ROW_NUMBER() OVER (ORDER BY ${columnData} DESC) AS xx,
        sym AS xx_label,
        ${columnData} AS yy
    FROM tradebot_position_stock
                    `)
                );
                tableChart = `chart._{{ii}}_tradebot_${grouping}_${ptype}`;
                return (`
-- chart - ${tableChart} - create
DROP TABLE IF EXISTS __tmp1;
CREATE TEMP TABLE __tmp1 AS SELECT * FROM (${sqlSelect}) ORDER BY xx;
${sqlChartCreate(tableChart)}
INSERT INTO ${tableChart} (datatype, options)
    SELECT
        'options' AS datatype,
        '${JSON.stringify(optionDict)}' AS options;
INSERT INTO ${tableChart} (
    datatype,
    options,
    series_index,
    series_label
)
    SELECT
        'series_label' AS datatype,
        JSON_OBJECT(
            'isDummy', is_dummy,
            'isHidden', is_hidden,
            'seriesColor', series_color
        ) AS options,
        xx AS series_index,
        series_label
    FROM __tmp1;
INSERT INTO ${tableChart} (datatype, xx, xx_label)
    SELECT
        'xx_label' AS datatype,
        xx,
        xx_label
    FROM __tmp1;
INSERT INTO ${tableChart} (
    datatype,
    series_index,
    xx,
    yy
)
    SELECT
        'yy_value' AS datatype,
        xx AS series_index,
        xx,
        ROUNDORZERO(yy, 4) AS yy
    FROM __tmp1;
                `);
            });
        }),
        (`
-- chart - tradebot_buysell_history
DROP TABLE IF EXISTS __tmp1;
CREATE TEMP TABLE __tmp1 AS
    SELECT
        series_color,
        printf(
            '%s %s',
            xx_label,
            series_label
        ) AS series_label,
        xx,
        xx_label,
        yy
    FROM (
        SELECT
            (CASE
                WHEN (subindustry LIKE '___+') THEN 3
                WHEN (subindustry LIKE '___-') THEN 1
                ELSE 2
            END) AS series_color,
            (
                buy_or_sell
                || ' '
                || sym
                || ' - '
                || company_name
            ) AS series_label,
            UNIXEPOCH(time_filled) AS xx,
            TIME(time_filled) AS xx_label,
            ROUNDORZERO(
                (
                    100 * 1.0 / asset_under_mgmt
                    * IIF(buy_or_sell = 'buy', 1, -1)
                    * order_value
                ),
                4
            ) AS yy
        FROM tradebot_account
        JOIN tradebot_buysell_history
        ORDER BY
            time_filled
    );
DROP TABLE IF EXISTS chart._{{ii}}_tradebot_buysell_history;
CREATE TABLE chart._{{ii}}_tradebot_buysell_history (
    datatype TEXT NOT NULL,
    series_index INTEGER,
    xx REAL,
    yy REAL,
    series_label REAL,
    xx_label TEXT,
    options TEXT
);
INSERT INTO chart._{{ii}}_tradebot_buysell_history (datatype, options)
    SELECT
        'options' AS datatype,
        '{
            "isBarchart": true,
            "title": "tradebot buy/sell history today\\n[ updated '
                || '${tradebotState.datenowUtc}'
                || ' ]",
            "xaxisTitle": "time",
            "xvalueConvert": "unixepochToTimeutc",
            "yaxisTitle": "buy/sell value as percentage of account",
            "yvalueSuffix": " %"
        }' AS options
    FROM tradebot_state;
INSERT INTO chart._{{ii}}_tradebot_buysell_history (
    datatype,
    options,
    series_index,
    series_label
)
    SELECT
        'series_label' AS datatype,
        JSON_OBJECT('seriesColor', series_color) AS options,
        __tmp1.rowid AS series_index,
        series_label
    FROM __tmp1;
INSERT INTO chart._{{ii}}_tradebot_buysell_history (
    datatype,
    series_index,
    xx,
    yy
)
    SELECT
        'yy_value' AS datatype,
        __tmp1.rowid AS series_index,
        xx,
        yy
    FROM __tmp1;
        `),
        [
            "1 day",
            "1 month"
        ].map(function (dateInterval) {
            let optionDict;
            let tableChart;
            let tableData;
            tableData = (
                dateInterval === "1 day"
                ? "tradebot_tech_intra_day"
                : "tradebot_tech_intra_month"
            );
            tableChart = (
                "chart._{{ii}}_tradebot_tech_intra_" +
                dateInterval.replace((
                    /\W/g
                ), "_")
            );
            optionDict = {
                title: (
                    "tradebot technical intraday - " +
                    dateInterval +
                    (
                        dateInterval === "1 day"
                        ? "\n[ updated " + tradebotState.datenowUtc + " ]"
                        : ""
                    )
                ),
                xaxisTitle: "date",
                xstep: (
                    dateInterval === "1 day"
                    ? 60
                    : 5 * 60
                ),
                xvalueConvert: "unixepochToTimeutc",
                yaxisTitle: "percent gain",
                yvalueSuffix: " %"
            };
            return (`
UPDATE ${tableData}
    SET
        xdate = UNIXEPOCH(xdate),
        tval = ROUNDORZERO(tval, 4);

-- chart - ${tableChart} - create
${sqlChartCreate(tableChart)}
INSERT INTO ${tableChart} (datatype, options)
    SELECT
        'options' AS datatype,
        '${JSON.stringify(optionDict)}' AS options;

INSERT INTO ${tableChart} (datatype, options, series_index, series_label)
    SELECT
        'series_label' AS datatype,
        JSON_OBJECT(
            'isHidden', NOT tname IN (
                -- '1a_stk_pct',
                -- '1b_stk_lmt',
                '2a_spy_prc'
            )
        ) AS options,
        rownum AS series_index,
        tname AS series_label
    FROM (
        SELECT
            ROW_NUMBER() OVER (ORDER BY tname) AS rownum,
            tname
        FROM (SELECT DISTINCT tname FROM ${tableData})
        WHERE
            tname IS NOT NULL
    );
INSERT INTO ${tableChart} (datatype, xx, xx_label)
    SELECT
        'xx_label' AS datatype,
        rownum AS xx,
        xdate AS xx_label
    FROM (
        SELECT
            ROW_NUMBER() OVER (ORDER BY xdate) AS rownum,
            xdate
        FROM (SELECT DISTINCT xdate FROM ${tableData})
    );
INSERT INTO ${tableChart} (datatype, series_index, xx, yy)
    SELECT
        'yy_value' AS datatype,
        series_index,
        xx_label AS xx,
        tval AS yy
    FROM (
        SELECT
            series_index,
            series_label,
            xx,
            xx_label
        FROM (
            SELECT
                series_index,
                series_label
            FROM ${tableChart}
            WHERE
                datatype = 'series_label'
        )
        JOIN (
            SELECT
                xx,
                xx_label
            FROM ${tableChart}
            WHERE
                datatype = 'xx_label'
        )
    )
    LEFT JOIN ${tableData} ON tname = series_label AND xdate = xx_label;
DELETE FROM ${tableChart} WHERE datatype = 'xx_label';
            `);
        }),
        (function () {
            let tableChart = `chart._{{ii}}_tradebot_tech_hist_1_year`;
            return (`
${sqlChartCreate(tableChart)}

-- table - ${tableChart} - insert
INSERT INTO ${tableChart} (datatype, options)
    SELECT
        'options' AS datatype,
        '{
            "title": "tradebot technical historical - 1 year",
            "xaxisTitle": "date",
            "xvalueConvert": "juliandayToDate",
            "yaxisTitle": "percent gain",
            "yvalueSuffix": " %"
        }' AS options;
INSERT INTO ${tableChart} (datatype, options, series_index, series_label)
    SELECT
        'series_label' AS datatype,
        IIF(value <= 3, '{}', '{"isHidden": 1}') AS options,
        value AS series_index,
        (CASE
            WHEN (value = 1) THEN
                'spy'
            WHEN (value = 2) THEN
                'spy predicted linear - 2 month window'
            WHEN (value = 3) THEN
                'spy predicted sine - 2 month window'
            WHEN (value = 4) THEN
                'spy predicted linear+sine - 2 month window'
            WHEN (value = 5) THEN
                'spy predicted linear - 6 month window'
            WHEN (value = 6) THEN
                'spy predicted sine - 6 month window'
            WHEN (value = 7) THEN
                'spy predicted linear+sine - 6 month window'
        END) AS series_label
    FROM GENERATE_SERIES(1, 7);
INSERT INTO ${tableChart} (datatype, xx, xx_label)
    SELECT
        'xx_label' AS datatype,
        JULIANDAY(xdate) AS xx,
        xdate AS xx_label
    FROM (
        SELECT DISTINCT
            xdate
        FROM tradebot_tech_histo
        ORDER BY ttt DESC
    );
INSERT INTO ${tableChart} (datatype, series_index, xx, yy)
    SELECT
        'yy_value' AS datatype,
        series_index,
        xx,
        yy
    FROM (SELECT xx, xx_label FROM ${tableChart} WHERE datatype = 'xx_label')
    LEFT JOIN (
        SELECT
            1 AS series_index,
            xdate,
            price_actual AS yy
        FROM tradebot_tech_histo
        --
        UNION ALL
        --
        SELECT
            2 AS series_index,
            xdate,
            price_linear_02 AS yy
        FROM tradebot_tech_histo
        --
        UNION ALL
        --
        SELECT
            3 AS series_index,
            xdate,
            price_sine_02 + __offset AS yy
        FROM tradebot_tech_histo
        JOIN (
            SELECT
                price_actual - price_sine_02 AS __offset
            FROM tradebot_tech_histo
            WHERE
                ttt = 1
        )
        --
        UNION ALL
        --
        SELECT
            4 AS series_index,
            xdate,
            price_predicted_02 AS yy
        FROM tradebot_tech_histo
        --
        UNION ALL
        --
        SELECT
            5 AS series_index,
            xdate,
            price_linear_06 AS yy
        FROM tradebot_tech_histo
        --
        UNION ALL
        --
        SELECT
            6 AS series_index,
            xdate,
            price_sine_06 + __offset AS yy
        FROM tradebot_tech_histo
        JOIN (
            SELECT
                price_actual - price_sine_06 AS __offset
            FROM tradebot_tech_histo
            WHERE
                ttt = 1
        )
        --
        UNION ALL
        --
        SELECT
            7 AS series_index,
            xdate,
            price_predicted_06 AS yy
        FROM tradebot_tech_histo
    ) ON xdate = xx_label
    WHERE yy IS NOT NULL;
DELETE FROM ${tableChart} WHERE datatype = 'xx_label';

-- table - ${tableChart} - normalize - yy
UPDATE ${tableChart}
    SET
        yy = yy * __inv
    FROM (
        SELECT
            100.0 / price_actual AS __inv
        FROM tradebot_tech_histo
        WHERE
            ttt = (SELECT MAX(ttt) FROM tradebot_tech_histo)
    );
UPDATE ${tableChart}
    SET
        yy = ROUND(yy - __avg, 4)
    FROM (
        SELECT
            MEDIAN(yy) AS __avg
        FROM ${tableChart}
        WHERE
            series_index = 1
    );
            `);
        }())
    ].flat().flat().map(function (sql, ii) {
        return sql.trim().replace((
            /\{\{ii\}\}/g
        ), String(ii).padStart(2, "0"));
    }).join("\n\n\n\n") + "\n");
    await onDbExec({});
    return true;
}

function domDivCreate(innerHTML) {
// this function will return div-element with rendered <innerHTML>
    let elem = document.createElement("div");
    elem.innerHTML = innerHTML;
    return elem;
}

function fileSave({
    buf,
    filename
}) {
// this function will save <buf> with given <filename>
    // create new blob
    const url = URL.createObjectURL(new Blob([
        buf
    ]));
    UI_FILE_SAVE.href = url;
    // cleanup this blob, not a later save's, to prevent memory-leak
    setTimeout(function () {
        URL.revokeObjectURL(url);
    }, 30000);
    UI_FILE_SAVE.download = filename.toLowerCase().replace((
        /[^0-9a-z]+/g
    ), "_").replace((
        /_([^_]+)$/
    ), (
        "_" +
        new Date().toISOString().slice(0, 10).replace((
            /-/g
        ), "") +
        ".$1"
    ));
    UI_FILE_SAVE.click();
}

async function init() {
    let isDemoDone;
    let isScriptFailed;
    let modeDemo = true;
    await sqlmathWebworkerInit({});
    [
        DB_CHART, DB_QUERY, DB_MAIN
    ] = await Promise.all([{
        dbName: "chart",
        filename: "file:dbchart?mode=memory&cache=shared",
        isDbchart: true
    }, {
        dbName: "query",
        filename: "file:dbquery?mode=memory&cache=shared",
        isDbquery: true
    }, {
        dbName: "main",
        filename: ":memory:",
        isDbmain: true
    }].map(async function (db) {
        db = Object.assign(noop(
            await dbOpenAsync({filename: db.filename})
        ), db);
        DB_DICT.set(db.dbName, db);
        return db;
    }));
    // attach db
    await Promise.all([
        DB_CHART, DB_QUERY
    ].map(async function (db) {
        await dbExecAsync({
            db: DB_MAIN,
            sql: `ATTACH DATABASE [${db.filename}] AS ${db.dbName};`
        });
    }));
    UI_FILE_OPEN.type = "file";
    UI_EDITOR = CodeMirror.fromTextArea(document.querySelector(
        "#sqliteEditor1"
    ), {
        extraKeys: {
            Tab: function (cm) {
                cm.replaceSelection(
                    new Array(cm.getOption("indentUnit") + 1).join(" ")
                );
            }
        },
        lineNumbers: true,
        lineWrapping: true,
        matchBrackets: true,
        mode: "text/x-mysql"
    });
    [
        ["#contentPanel1", "click", onTocToggle],
        ["#tocPanel1", "click", onDbAction],
        ["#tocPanel1", "click", onTocToggle],
        ["#tocToggle1", "click", onTocToggle],
        [".dbExec", "click", onDbExec],
        [".dbcrudExec", "click", onDbcrudExec],
        [".modalCancel", "click", onModalClose],
        [".modalClose", "click", onModalClose],
        ["body", "click", onContextmenu],
        ["body", "contextmenu", onContextmenu],
        [UI_FILE_OPEN, "change", onDbAction],
        [document, "keydown", onKeyDown],
        [document, "touchcancel", onTouchLongpress],
        [document, "touchend", onTouchLongpress],
        [document, "touchmove", onTouchLongpress],
        [document, "touchstart", onTouchLongpress],
        [window, "hashchange", uitableInitWithinView],
        [window, "resize", onResize]
    ].forEach(function ([
        selector, evt, listener
    ]) {
        if (typeof selector !== "string") {
            selector.addEventListener(evt, listener);
            return;
        }
        selector = document.querySelectorAll(selector);
        assertOrThrow(selector.length > 0);
        selector.forEach(function (elem) {
            elem.addEventListener(evt, listener);
        });
    });
    window.onscroll = uitableInitWithinView;
    window.scroll({
        behavior: "smooth",
        top: 0
    });
    // animate timer before the url-param loads, whose charts animate [r16]
    // REVIEWED r15: wakes every 16 ms even idle, battery only
    setInterval(function () {
        UI_ANIMATE_DATENOW = Date.now();
        UI_ANIMATE_LIST = UI_ANIMATE_LIST.filter(function (
            svgAnimateStep
        ) {
            return !svgAnimateStep();
        });
    }, 16);
    // REVIEWED r20, r30: url and default demo loads skip DB_EXEC_CHAIN -
    // a db opened during the fetch is replaced; rare
    await Promise.all(location.search.slice(1).split("&").map(async function (
        elem
    ) {
        let [
            key, ...valList
        ] = elem.split("=");
        // raw value, not URLSearchParams - it turned "+" into space, and
        // decoding first broke encoded "#" and "?" (fetch takes %xx as is)
        let val = valList.join("=");
        switch (key) {
        // REVIEWED r15: ?demo= before ?sqlDb= awaits the old DB_INIT
        case "demo":
            switch (val) {
            case "demoDefault":
            case "demoTradebot":
                modeDemo = undefined;
                await DB_INIT;
                // false if its db failed to load - init then renders anyway
                isDemoDone = await (
                    val === "demoDefault"
                    ? demoDefault()
                    : demoTradebot()
                );
                return;
            }
            return;
        case "jsScript":
            // same-origin only [reviewed 2026-11-09] - every gh-pages project
            // page shares the kaizhu256.github.io origin, so a crafted link
            // to an outside script would get all of their storage
            // malformed url, e.g. "//" - skip it, a throw would abort init
            try {
                key = new URL(val, location.href).origin;
            } catch (ignore) {
                key = "";
            }
            // file:// and data: both have origin "null" [r20]
            if (key !== location.origin || key === "null") {
                return;
            }
            modeDemo = undefined;
            key = document.createElement("script");
            key.src = val;
            if (val.endsWith(".mjs")) {
                key.type = "module";
            }
            document.head.appendChild(key);
            return;
        case "modeExpert":
            if (val === "1") {
                document.head.insertAdjacentHTML(
                    "beforeend",
                    "<style>#contentPanel1 th {max-width: 48px;}</style>"
                );
            }
            return;
        case "sqlDb":
            modeDemo = undefined;
            // uiTryCatch shows a failed fetch in the error panel, and never
            // rejects, so init still runs the editor
            DB_INIT = uiTryCatch(async function () {
                let res;
                res = await fetch(val);
                if (!res.ok) {
                    throw new Error(`sqlDb - fetch ${val} - ${res.status}`);
                }
                await dbFileLoadAsync({
                    db: DB_MAIN,
                    dbData: await res.arrayBuffer()
                });
            });
            return;
        case "sqlScript":
            modeDemo = undefined;
            await DB_INIT;
            // uiTryCatch shows a failed fetch in the error panel, else a bad
            // link loads the server 404 page into the editor as sql
            isScriptFailed = await uiTryCatch(async function () {
                let res;
                res = await fetch(val);
                if (!res.ok) {
                    throw new Error(`sqlScript - fetch ${val} - ${res.status}`);
                }
                UI_EDITOR.setValue(await res.text());
            }) === false;
            return;
        }
    }));
    // ?demo= already ran its script - do not run it again
    if (isDemoDone) {
        return;
    }
    if (!modeDemo) {
        // failed load - skip the run, whose first step would fade out the
        // load error after ~0.5s, leaving an empty app and no explanation
        if (await DB_INIT === false || isScriptFailed) {
            // render without onDbExec, which would fade the error - import
            // needs DB_MAIN.dbtableList, which only uiRenderDb sets
            await uiRenderDb();
            return;
        }
        await onDbExec({});
        return;
    }
    if (
        await demoTradebot()
    ) {
        return;
    }
    await demoDefault();
}

function onContextmenu(evt) {
// this function will handle contextmenu-event
    let baton;
    let {
        clientX,
        clientY,
        ctrlKey,
        metaKey,
        shiftKey,
        target,
        type
    } = evt;
    // contextmenu - left-click
    if (type !== "contextmenu") {
        // action only while shown - a click mid-fade queued it twice [r16]
        if (
            UI_CONTEXTMENU.style.visibility === "visible" &&
            target.closest(".contextmenuElem")
        ) {
            onDbAction(evt);
        }
        uiFadeOut(UI_CONTEXTMENU);
        return;
    }
    // right-click - REVIEWED: modifier-click and shift+f10 keep the
    // native menu, so crud has no keyboard path
    if (ctrlKey || metaKey || shiftKey) {
        return;
    }
    target = target.closest(`.tocElemA[data-dbtype], tr[data-dbtype="row"]`);
    // keep the native menu off targets - editor paste needs it [r15]
    if (!target) {
        uiFadeOut(UI_CONTEXTMENU);
        return;
    }
    evt.preventDefault();
    // a copy - rowid on the shared baton retargeted a queued delete [r16]
    UI_CONTEXTMENU_BATON = Object.assign(
        {},
        DBTABLE_DICT.get(target.dataset.hashtag),
        {dict: DBTABLE_DICT, rowid: target.dataset.rowid}
    );
    baton = UI_CONTEXTMENU_BATON;
    UI_CONTEXTMENU.querySelectorAll(
        ".contextmenuDivider, .contextmenuElem"
    ).forEach(function ({
        dataset,
        style
    }) {
        style.display = "none";
        if (dataset.dbtype !== target.dataset.dbtype) {
            return;
        }
        switch (dataset.action) {
        case "dbDetach":
            if (baton.isDbmain) {
                return;
            }
            break;
        case "dbrowDelete":
        case "dbrowUpdate":
            // WITHOUT ROWID rows render rowid "null" - no row to act on
            if (
                target.dataset.rowid === undefined ||
                target.dataset.rowid === "null"
            ) {
                return;
            }
            break;
        }
        style.display = "block";
    });
    UI_CONTEXTMENU.children[0].innerHTML = (
        "crud operation for:<br>" +
        stringHtmlSafe(baton.dbtableFullname || "script editor")
    );
    uiFadeIn(UI_CONTEXTMENU);
    UI_CONTEXTMENU.style.left = Math.max(0, Math.min(
        clientX,
        window.innerWidth - UI_CONTEXTMENU.offsetWidth - 10
    )) + "px";
    UI_CONTEXTMENU.style.top = Math.max(0, Math.min(
        clientY,
        window.innerHeight - UI_CONTEXTMENU.offsetHeight - 20
    )) + "px";
}

async function onDbAction(evt) {
// this function will handle toc, contextmenu and file-open actions
    let action;
    let baton = evt.baton || UI_CONTEXTMENU_BATON;
    let colList;
    let columntypeList;
    let crud;
    let data;
    let target;
    let title;
    target = evt.target.closest("[data-action]") || evt.target;
    action = evt.action || target.dataset.action;
    if (!action) {
        return;
    }
    // fast actions that do not require loading
    switch (target !== UI_FILE_OPEN && action) {
    case "dbAttach":
    case "dbOpen":
    case "dbscriptOpen":
    case "dbtableImportCsv":
    case "dbtableImportJson":
    case "dbtableImportTsv":
        UI_FILE_OPEN.dataset.action = action;
        UI_FILE_OPEN.value = "";
        UI_FILE_OPEN.click();
        return;
    case "dbcolumnAdd":
    case "dbcolumnDrop":
    case "dbcolumnRename":
    case "dbrowInsert":
    case "dbrowUpdate":
    case "dbtableRename":
        title = target.textContent.trim().replace(/\s+/g, " ");
        UI_CRUD.querySelector(".modalTitle").innerHTML = (
            `${stringHtmlSafe(baton.dbtableFullname)}<br>` +
            `${stringHtmlSafe(title)}`
        );
        data = baton.colList.slice(1).map(function (col) {
            return `<option>${stringHtmlSafe(col)}</option>`;
        }).join("");
        // one row per placeholder, hidden until its action's entry shows it
        UI_CRUD.querySelector("tbody").innerHTML = [
            "new_table", "selected_column", "new_column"
        ].map(function (name) {
            return (`
<tr class="crudInput-${name}" style="display: none;">
    <td><span class="crudLabel">{{${name}}}</span></td>
    <td class="tdInput">
            `) + (
                name === "selected_column"
                ? `<select class="crudInput">${data}</select>`
                : `<input class="crudInput" type="text" value="${name}_1">`
            ) + (`
    </td>
</tr>
            `);
        }).join("");
        // every template is built, then one picked - nothing may throw [r26];
        // keyed by action, not label text: [sql, inputs shown]
        crud = {
            dbcolumnAdd: [`-- column - add
ALTER TABLE
    ${baton.dbtableFullname}
ADD
    "{{new_column}}" TEXT NOT NULL DEFAULT '';`, ".crudInput-new_column"],
            dbcolumnDrop: [`-- column - drop
ALTER TABLE
    ${baton.dbtableFullname}
DROP COLUMN
    "{{selected_column}}";`, ".crudInput-selected_column"],
            dbcolumnRename: [`-- column - rename
ALTER TABLE
    ${baton.dbtableFullname}
RENAME
    "{{selected_column}}"
TO
    "{{new_column}}";`, ".crudInput-new_column, .crudInput-selected_column"],
            dbrowInsert: [(
                `-- row - insert
INSERT INTO ${baton.dbtableFullname} (\n    ` +
                baton.colListWritable.slice(1).map(sqlIdQuote).join(",\n    ") +
                "\n) VALUES (\n" +
                "    NULL,\n".repeat(
                    Math.max(0, baton.colListWritable.length - 2)
                ) +
                "    NULL\n);"
            ), ""],
            dbrowUpdate: [(
                `-- row - update
UPDATE
    ${baton.dbtableFullname}
SET
` +
                baton.colListWritable.slice(1).map(function (col) {
                    return `    ${sqlIdQuote(col)} = NULL`;
                }).join(",\n") +
                `
WHERE
    rowid IN (${String(baton.rowid).replace((/[^0-9\-]/g), "")});`
            ), ""],
            dbtableRename: [`-- table - rename
ALTER TABLE
    ${baton.dbtableFullname}
RENAME TO
    "{{new_table}}";`, ".crudInput-new_table"]
        };
        crud = crud[action];
        UI_CRUD.querySelector("textarea").value = crud[0] + "\n";
        if (crud[1]) {
            UI_CRUD.querySelectorAll(crud[1]).forEach(function (elem) {
                elem.style.display = "table-row";
            });
        }
        uiFadeIn(UI_CRUD);
        // new modal-open - a crud run queued from an earlier one must skip
        UI_CRUD_BATON = baton;
        return;
    }
    // slow actions that require loading
    if (!evt.modeTryCatch) {
        evt.modeTryCatch = true;
        evt.preventDefault();
        // queue like onDbExec, keeping this click's baton - concurrent renders
        // cross-wired batons [r15]; REVIEWED r16: 3 queue sites, no helper
        evt.baton = baton;
        // and its action and file - UI_FILE_OPEN is re-read at dequeue [r17]
        evt.action = action;
        evt.file = target.files?.[0];
        DB_EXEC_CHAIN = DB_EXEC_CHAIN.then(function () {
            return uiTryCatch(onDbAction, evt);
        });
        await DB_EXEC_CHAIN;
        return;
    }
    if (target === UI_FILE_OPEN && !evt.file) {
        return;
    }
    // a menu action queued across a re-render hits a recreated table [r29]
    if (
        UI_CONTEXTMENU.contains(target) &&
        baton.hashtag &&
        baton.dict !== DBTABLE_DICT
    ) {
        throw new Error("tables were re-rendered - right-click again");
    }
    switch (target === UI_FILE_OPEN && action) {
    case "dbAttach":
    case "dbOpen":
        await (
            action === "dbOpen"
            ? dbFileLoadAsync
            : dbFileAttachAsync
        )({
            db: DB_MAIN,
            dbData: await evt.file.arrayBuffer()
        });
        await uiRenderDb();
        return;
    case "dbscriptOpen":
        UI_EDITOR.setValue(
            await evt.file.text()
        );
        return;
    case "dbtableImportCsv":
    case "dbtableImportJson":
    case "dbtableImportTsv":
        await dbTableImportAsync({
            db: DB_MAIN,
            mode: action.replace("dbtableImport", "").toLowerCase(),
            tableName: dbNameNext(
                // keys are sqlIdQuote'd, so match "..." not [...]
                "\"__file{{ii}}\"",
                new Set(DB_MAIN.dbtableList.keys())
            ).slice(1, -1),
            textData: (
                await evt.file.text()
            )
        });
        await uiRenderDb();
        return;
    }
    switch (action) {
    case "dbDetach":
        if (!window.confirm(
            "are you sure you want to detach and close database" +
            ` ${baton.dbName} ?`
        )) {
            return;
        }
        await dbExecAsync({
            db: DB_MAIN,
            sql: `DETACH ${baton.dbName};`
        });
        // REVIEWED r16: uiRenderDb closes it again, a no-op
        await dbCloseWhenIdle(baton.db);
        await uiRenderDb();
        return;
    case "dbExport":
    case "dbExportMain":
        data = (
            action === "dbExportMain"
            ? DB_MAIN
            : baton.db
        );
        fileSave({
            buf: await dbFileSaveAsync({db: data}),
            filename: `sqlite_database_${data.dbName}.sqlite`
        });
        return;
    case "dbRefresh":
        await uiRenderDb();
        return;
    // qualified names on DB_MAIN - unqualified hit a TEMP shadow [r24];
    // BigInt - a rowid past 2^53 arrives as an exact string [r25]
    case "dbrowDelete":
        if (!window.confirm(
            `are you sure you want to delete row with rowid = ${baton.rowid}` +
            ` in table ${baton.dbtableFullname} ?`
        )) {
            return;
        }
        await dbExecAsync({
            db: DB_MAIN,
            sql: (`
DELETE FROM ${baton.dbtableFullname}
    WHERE rowid IN (${BigInt(baton.rowid)});
            `)
        });
        await uiRenderDb();
        return;
    case "dbscriptSave":
        fileSave({
            buf: UI_EDITOR.getValue(),
            filename: `sqlite_script.sql`
        });
        return;
    case "dbtableDrop":
        if (!window.confirm(
            `are you sure you want to drop table ${baton.dbtableFullname} ?`
        )) {
            return;
        }
        await dbExecAsync({
            db: DB_MAIN,
            sql: `DROP TABLE ${baton.dbtableFullname};`
        });
        await uiRenderDb();
        return;
    case "dbtableSaveCsv":
        data = await dbExecAsync({
            db: DB_MAIN,
            responseType: "list",
            sql: `SELECT ${baton.sqlRowid}, * FROM ${baton.dbtableFullname};`
        });
        data = data[0] || [];
        // header from the query - a re-created table changed columns [r22]
        data = csvFromListofList({
            colList: data.shift() || baton.colList,
            rowList: data
        });
        fileSave({
            buf: data || "",
            filename: `sqlite_table_${baton.dbtableName}.csv`
        });
        return;
    // REVIEWED r24: csv and sql share a fetch - left
    case "dbtableSaveJson":
        data = await dbExecAsync({
            db: DB_MAIN,
            sql: `SELECT ${baton.sqlRowid}, * FROM ${baton.dbtableFullname};`
        });
        fileSave({
            buf: JSON.stringify(data[0] || []),
            filename: `sqlite_table_${baton.dbtableName}.json`
        });
        return;
    case "dbtableSaveSql":
        data = await dbExecAsync({
            db: DB_MAIN,
            responseType: "list",
            sql: `SELECT ${baton.sqlRowid}, * FROM ${baton.dbtableFullname};`
        });
        data = data[0] || [];
        // header from the query, as for csv [r22, r23]
        colList = data.shift() || baton.colList;
        columntypeList = await dbExecAsync({
            db: DB_MAIN,
            responseType: "list",
            sql: (
                `SELECT ` +
                colList.map(function (col, ii) {
                    // col 0 is rowid - NULL for a WITHOUT ROWID table
                    return "COLUMNTYPE(" + (
                        ii === 0
                        ? baton.sqlRowid.replace(" AS rowid", "")
                        : sqlIdQuote(col)
                    ) + ")";
                }).join(",") +
                ` FROM ${baton.dbtableFullname};`
            )
        });
        columntypeList = columntypeList[0][1];
        data = (
            String(`
-- DROP TABLE __sqlite_table_01;
-- IF OBJECT_ID('tempdb..#tmp1') IS NOT NULL DROP TABLE #tmp1;
-- SELECT * FROM __sqlite_table_01;
-- ALTER TABLE __sqlite_table_01 RENAME TO __sqlite_table_02;
-- EXEC sp_rename '__sqlite_table_01', '__sqlite_table_02';
            `).trim() +
            `\nCREATE TABLE __sqlite_table_01 (\n` +
            // REVIEWED: reserved words stay bare - quoting needs a dialect
            colList.map(function (col) {
                return (
                    col.replace((/\W/g), "_").replace((/^\d/), "_$&") || "_"
                );
            }).reduce(function (list, col) {
                // dedupe against every name produced so far - "名前" and
                // "住所" both became "__", a csv round-trip repeats "rowid",
                // and a suffix can itself collide, e.g. a, a, a_2
                let ii = 1;
                let name = col;
                // case-insensitive, as sql names are: Total_Count, total_count
                while (list.some(function (name2) {
                    return name2.toLowerCase() === name.toLowerCase();
                })) {
                    ii += 1;
                    name = col + "_" + ii;
                }
                list.push(name);
                return list;
            }, []).map(function (col, ii) {
                switch (columntypeList[ii]) {
                case 2: // SQLITE_FLOAT
                    return `    ${col} FLOAT(53)`;
                case 3: // SQLITE_TEXT
                    return `    ${col} VARCHAR(255)`;
                case 11: // SQLITE_COLUMNTYPE_INTEGER_BIG
                    return `    ${col} BIGINT`;
                case 13: // SQLITE_COLUMNTYPE_TEXT_BIG
                    return `    ${col} TEXT`;
                default:
                    return `    ${col} INTEGER`;
                }
            }).join(",\n") +
            `\n);\n` +
            data.map(function (rowList) {
                return (
                    `INSERT INTO __sqlite_table_01 VALUES (` +
                    rowList.map(function (val) {
                        // type by value, not by COLUMNTYPE, a column aggregate:
                        // a mixed text/integer column crashed .replace, and
                        // one blob exported every text value as NULL
                        switch (typeof val) {
                        case "number":
                            return val;
                        case "string":
                            return "'" + val.replace((/'/g), "''") + "'";
                        default:
                            return "NULL";
                        }
                    }).join(",") +
                    `);\n`
                );
            }).join("")
        );
        fileSave({
            buf: data,
            filename: `sqlite_table_${baton.dbtableName}.sql`
        });
        return;
    }
    throw new Error(`onDbAction - invalid action ${action}`);
}

async function onDbExec({
    modeTryCatch
}) {
// this function will
// 1. exec sql-command in webworker
// 2. save query-result
// 3. ui-render sql-queries to html
    let dbqueryList;
    if (!modeTryCatch) {
        // its baton may name a result table this run re-creates [r15]
        uiFadeOut(UI_CONTEXTMENU);
        // queue runs, e.g. double ctrl+enter or a crud refresh during a
        // long run, else they race to re-create result_00 and one fails
        // "table already exists"; uiTryCatch never rejects, so chain holds
        DB_EXEC_CHAIN = DB_EXEC_CHAIN.then(function () {
            return uiTryCatch(onDbExec, {
                modeTryCatch: true
            });
        });
        await DB_EXEC_CHAIN;
        return;
    }
    uiFadeOut(document.querySelector("#errorPanel1"));
    // 3. ui-render in finally - also after a failed script or save, as the
    // chart and query tables are gone
    try {
        // clear panels before the drop, keeping their height - a scroll mid-run
        // queried dropped tables; height kept, else scroll clamps [r19-r31]
        [
            "#dbchartList1", "#dbqueryList1", "#dbtableList1"
        ].forEach(function (selector) {
            const elem = document.querySelector(selector);
            elem.style.minHeight = elem.offsetHeight + "px";
            elem.innerHTML = "";
        });
        await Promise.all([
            DB_CHART, DB_QUERY
        ].map(async function (db) {
            let sqlCleanup = noop(
                await dbExecAsync({
                    db,
                    sql: (`
SELECT
            group_concat(
                'DROP ' || UPPER(type) || ' IF EXISTS "' ||
                REPLACE(name, '"', '""') || '";',
                ''
            ) AS sql
        FROM sqlite_master
        WHERE
            -- views too, else CREATE VIEW query.v failed every later run [r15]
            type IN ('table', 'view')
            -- sqlite_sequence etc. may not be dropped - one AUTOINCREMENT
            -- table made every later execute fail; IF EXISTS covers fts shadows
            AND name NOT LIKE 'sqlite!_%' ESCAPE '!';
                    `)
                })
            )[0][0].sql || "";
            await dbExecAsync({
                db,
                sql: sqlCleanup
            });
        }));
        // REVIEWED r30: ints past 2^53 land TEXT in result_NN
        // 1. exec sql-command in webworker
        dbqueryList = await dbExecAsync({
            db: DB_MAIN,
            responseType: "list",
            sql: UI_EDITOR.getValue()
        });
        // 2. save query-result
        await Promise.all(dbqueryList.map(async function (rowList, ii) {
            let colList = rowList.shift().map(function (col, ii) {
                // bugfix - escape special-character in col
                return `value->>${ii} AS ${sqlIdQuote(col)}`;
            }).join(",");
            await dbExecAsync({
                bindList: {
                    tmp1: JSON.stringify(rowList)
                },
                db: DB_QUERY,
                sql: (`
CREATE TABLE result_${String(ii).padStart(2, "0")} AS
    SELECT
        ${colList}
    FROM JSON_EACH($tmp1);
                `)
            });
        }));
    } finally {
        // failed mid-BEGIN - roll back, else its write lock failed every later
        // run [r20]; REVIEWED r22: also drops a deliberate open transaction
        if (dbqueryList === undefined) {
            await dbExecAsync({
                db: DB_MAIN,
                sql: "ROLLBACK;"
            }).catch(noop);
        }
        await uiRenderDb();
    }
}

async function onDbcrudExec({
    modeTryCatch
}) {
// this function will exec crud operation
    const crudBaton = UI_CRUD_BATON;
    let sql;
    if (!modeTryCatch) {
        // queue like onDbExec; a double ctrl+enter or ok ran the crud sql
        // twice, e.g. 2 rows inserted - a run skips once its modal closed,
        // or was closed and reopened, else it ran unconfirmed sql
        DB_EXEC_CHAIN = DB_EXEC_CHAIN.then(function () {
            if (
                UI_CRUD.style.visibility !== "visible" ||
                UI_CRUD_BATON !== crudBaton
            ) {
                return;
            }
            return uiTryCatch(onDbcrudExec, {
                modeTryCatch: true
            });
        });
        await DB_EXEC_CHAIN;
        return;
    }
    // its table was re-rendered since the right-click [r29, r30]
    if (crudBaton.dict !== DBTABLE_DICT) {
        throw new Error("tables were re-rendered - right-click again");
    }
    // only "{{name}}", always quoted - the bare form ran sql [r23];
    // REVIEWED r25: a name that IS a placeholder hits the wrong object
    sql = document.querySelector("#crudPanel1 textarea").value.replace((
        /"\{\{(\w+?)\}\}"/g
    ), function (match0, name) {
        let val = document.querySelector(
            `#crudPanel1 .crudInput-${name} .crudInput`
        );
        if (!val) {
            return match0;
        }
        val = (
            val.tagName === "SELECT"
            ? val.selectedOptions[0]?.textContent ?? ""
            : val.value.trim()
        );
        return sqlIdQuote(val);
    });
    await dbExecAsync({
        db: DB_MAIN,
        sql
    });
    await uiRenderDb();
    // a modal opened while this ran is not ours to close
    if (UI_CRUD_BATON !== crudBaton) {
        return;
    }
    uiFadeOut(UI_CRUD);
}

function onKeyDown(evt) {
// this function will handle event keydown
    // lower-case - caps lock gives "S", and the browser's own save opened
    let key = String(evt.key).toLowerCase();
    // non-latin layout, e.g. russian "ы" on the s key - use the key position,
    // else the browser's own save or open runs; evt.key first, for dvorak
    if (!(/^[a-z]$/).test(key) && (/^Key[A-Z]$/).test(evt.code)) {
        key = evt.code.slice(3).toLowerCase();
    }
    switch (evt.key) {
    case "Escape":
        uiFadeOut(document.querySelector("#errorPanel1"));
        uiFadeOut(UI_CONTEXTMENU);
        // close crud-modal [r17]
        uiFadeOut(UI_CRUD);
        return;
    }
    // held key - auto-repeat queued ~15 runs or downloads a second [r19]
    if (evt.repeat) {
        return;
    }
    // shift+letter is another shortcut, e.g. firefox ctrl+shift+s
    // not altKey - windows AltGr is ctrl+alt [r18]
    switch (
        (evt.ctrlKey || evt.metaKey) &&
        !evt.altKey &&
        !(evt.shiftKey && key.length === 1) &&
        key
    ) {
    // database - execute
    // lower-case, as key is lowered above - "Enter" never matched
    case "enter":
        // crud modal open - run its sql, not the main editor script
        if (UI_CRUD.style.visibility === "visible") {
            onDbcrudExec({});
            return;
        }
        onDbExec({});
        return;
    // database - open
    case "o":
        evt.preventDefault();
        document.querySelector("button[data-action='dbOpen']").click();
        return;
    // database - save
    case "s":
        evt.preventDefault();
        document.querySelector("button[data-action='dbExportMain']").click();
        return;
    }
}

function onModalClose({
    currentTarget
}) {
// this function will close current modal
    uiFadeOut(currentTarget.closest(".modalPanel"));
}

function onResize() {
// this function will handle resize-event
    // rebuild charts on a width change only - a height-only resize, e.g. ios
    // url-bar collapsing on scroll, would rebuild every chart and lose zoom
    if (window.innerWidth !== UI_RESIZE_WIDTH) {
        UI_RESIZE_WIDTH = window.innerWidth;
        for (const elem of document.querySelectorAll(
            "#dbchartList1 .contentElem"
        )) {
            // REVIEWED r15: a failed chart re-shows its error on resize
            elem.dataset.init = "0";
        }
    }
    uitableInitWithinView({});
}

function onTocToggle(evt) {
// this function will open or close #tocPanel1 drawer on narrow screen:
// #tocToggle1 toggles it, a toc-link or #contentPanel1 closes it
// REVIEWED r17: toc buttons leave it open, by design
    let isOpen = false;
    if (evt.currentTarget.id === "tocToggle1") {
        isOpen = !document.body.classList.contains("tocOpen");
    } else if (
        evt.currentTarget.id === "tocPanel1" && !evt.target.closest("a")
    ) {
        return;
    }
    document.body.classList.toggle("tocOpen", isOpen);
    document.getElementById("tocToggle1").setAttribute(
        "aria-expanded",
        String(isOpen)
    );
}

function onTouchLongpress(evt) {
// this function will open contextmenu on touch long-press,
// since ios safari never fires contextmenu-event
    const touch = evt.touches[0];
    // ignore wobble under 10px - ios fires touchmove for a held finger
    if (evt.type === "touchmove" && touch && Math.hypot(
        touch.clientX - UI_LONGPRESS_XY[0],
        touch.clientY - UI_LONGPRESS_XY[1]
    ) < 10) {
        return;
    }
    // any other touch event ends a pending long-press
    clearTimeout(UI_LONGPRESS_TIMER);
    switch (evt.type) {
    case "touchend":
        // swallow click after long-press, else it hides the contextmenu
        if (UI_LONGPRESS_FIRED) {
            evt.preventDefault();
        }
        return;
    case "touchstart":
        UI_LONGPRESS_FIRED = false;
        // arm only on contextmenu targets, else a long-press on a button or
        // the editor would swallow its tap
        if (
            evt.touches.length !== 1 ||
            !evt.target.closest(`.tocElemA[data-dbtype], tr[data-dbtype="row"]`)
        ) {
            return;
        }
        UI_LONGPRESS_XY = [
            touch.clientX, touch.clientY
        ];
        UI_LONGPRESS_TIMER = setTimeout(function () {
            // re-rendered meanwhile - its hashtag now names another table
            if (!evt.target.isConnected) {
                return;
            }
            UI_LONGPRESS_FIRED = true;
            onContextmenu({
                clientX: touch.clientX,
                clientY: touch.clientY,
                preventDefault: noop,
                target: evt.target,
                type: "contextmenu"
            });
        }, 500);
        return;
    }
}

function sqlIdQuote(name) {
// this function will quote sql-identifier <name>, so names with space, "]"
// or a keyword do not break or inject into generated sql
    return "\"" + String(name).replace((/"/g), "\"\"") + "\"";
}

function stringHtmlSafe(str) {
// this function will make <str> html-safe
// https://stackoverflow.com/questions/7381974
    return String(str ?? "").replace((
        /&/gu
    ), "&amp;").replace((
        /</gu
    ), "&lt;").replace((
        />/gu
    ), "&gt;").replace((
        /"/gu
    ), "&quot;");
}

function svgAnimate(elem, attrDict, mode) {
// this function will animate <elem> with given <fxattr> in <attrDict>
    let {
        children,
        fx_rotate,
        fx_seriesShape,
        nodeName
    } = elem;
    let datebeg = UI_ANIMATE_DATENOW;
    let fxstateDict = {};
    let {
        translateY
    } = attrDict;
    // skip animation if hidden - REVIEWED r15: no caller passes hidden
    if (attrDict.visibility === "hidden") {
        svgAttrSet(elem, attrDict);
        return;
    }
    Object.entries(attrDict).forEach(function ([
        fxattr, fxend
    ]) {
        let dpathList;
        let fxattr2 = "fx_" + fxattr;
        let fxbeg;
        let fxstate;
        fxend = svgCrisp(fxattr, fxend);
        fxstate = {
            fxend
        };
        switch (fxattr) {
        case "d":
            dpathList = fxend.split(" ");
            fxbeg = noop(elem.getAttribute(fxattr) ?? "").split(" ");
            if (fxbeg.length !== dpathList.length) {
                elem.setAttribute(fxattr, fxend);
                return;
            }
            fxstate.dpathList = dpathList;
            fxstate.fxbeg = fxbeg;
            fxstateDict[fxattr] = fxstate;
            break;
        case "height":
        case "translateX":
        case "translateY":
        case "width":
        case "x":
        case "y":
            fxstate.fxbeg = Number(
                elem[fxattr2] ?? elem.getAttribute(fxattr) ?? 0
            );
            if (fx_seriesShape) {
                fxstate.fxbeg = fxstate.fxbeg || fxend;
            }
            fxstateDict[fxattr] = fxstate;
            break;
        case "visibility":
            elem.setAttribute(fxattr, fxend);
            return;
        default:
            throw new Error(`svgAnimate - invalid attribute - ${fxattr}`);
        }
    });
    function svgAnimateStep() {
        let fxprg = 1;
        let isDone = datebeg + UI_ANIMATE_DURATION <= UI_ANIMATE_DATENOW;
        // superseded by a newer animation of this elem - drop from list
        if (svgAnimateStep.isCancelled) {
            return true;
        }
        // animate - linear fxnow, fxprg
        if (!isDone) {
            fxprg = (
                UI_ANIMATE_DURATION_INV *
                (UI_ANIMATE_DATENOW - datebeg)
            );
            if (mode === "easeout") {
                fxprg = Math.sqrt(fxprg);
            }
        }
        Object.entries(fxstateDict).forEach(function ([
            fxattr, fxstate
        ]) {
            let {
                dpathList,
                fxbeg,
                fxend
            } = fxstate;
            let fxattr2 = "fx_" + fxattr;
            let fxnow = fxend;
            switch (fxattr) {
            // Perform the next step of the animation on "d"
            case "d":
                // interpolate fxnow from dpathList, fxbeg
                if (!isDone) {
                    fxnow = fxbeg.map(function (char, ii) {
                        let num;
                        if ("CLMZ".indexOf(char) !== -1) {
                            return char;
                        }
                        num = Number(char);
                        return num + fxprg * (dpathList[ii] - num);
                    }).join(" ");
                }
                elem.setAttribute("d", fxnow);
                return;
            case "height":
            case "translateX":
            case "translateY":
            case "width":
            case "x":
            case "y":
                if (!isDone) {
                    fxnow = fxbeg + fxprg * (fxend - fxbeg);
                }
                elem[fxattr2] = fxnow;
                if (fx_seriesShape || translateY !== undefined) {
                    return;
                }
                if (fxattr === "x" && nodeName === "text") {
                    Array.from(children).forEach(function (child) {
                        child.setAttribute("x", fxnow);
                    });
                }
                elem.setAttribute(fxattr, fxnow);
                return;
            }
        });
        if (fx_seriesShape) {
            elem.setAttribute("d", svgShapeDraw(
                fx_seriesShape,
                elem.fx_x,
                elem.fx_y,
                elem.fx_width,
                elem.fx_height
            ));
        }
        if (translateY !== undefined) {
            elem.setAttribute(
                "transform",
                `translate(${elem.fx_translateX},${elem.fx_translateY})`
            );
        }
        // after the loop - y follows x, so the pivot lagged a frame [r18]
        if (fx_rotate && elem.fx_x !== undefined) {
            elem.setAttribute(
                "transform",
                `rotate(-15 ${elem.fx_x} ${elem.fx_y || 0})`
            );
        }
        return isDone;
    }
    svgAnimateCancel(elem);
    elem.fx_step = svgAnimateStep;
    svgAnimateStep();
    UI_ANIMATE_LIST.push(svgAnimateStep);
}

function svgAnimateCancel(elem) {
// this function will cancel <elem>'s running animation, by flag - a
// UI_ANIMATE_LIST.filter per call made each redraw O(n^2) in markers
    if (elem.fx_step) {
        elem.fx_step.isCancelled = true;
    }
}

function svgAttrSet(elem, attrDict = {}) {
// this function will set-attribute items in <attrDict> to <elem>
    if (typeof elem === "string") {
        elem = document.createElementNS("http://www.w3.org/2000/svg", elem);
    }
    Object.entries(attrDict).forEach(function ([
        key, val
    ]) {
        if (val !== null && val !== undefined) {
            // crispify height, width, x, y; other keys pass through [r28]
            switch (key) {
            case "height":
            case "width":
            case "x":
            case "y":
                val = svgCrisp(key, val);
                elem["fx_" + key] = val;
                break;
            }
            elem.setAttribute(key, val);
        }
    });
    return elem;
}

function svgCrisp(key, val) {
// this function will snap height, width, x, y of <val> to whole pixels, x and
// y offset by CRISPX, CRISPY, so 1px strokes stay sharp; other keys unchanged
    switch (key) {
    case "height":
    case "width":
        return Math.round(val);
    case "x":
        return Math.round(val) + CRISPX;
    case "y":
        return Math.round(val) + CRISPY;
    }
    return val;
}

function svgShapeDraw(seriesShape, x, y, w, h) {
// this function will create svg-dpath for given <seriesShape>
    let tmp;
    switch (seriesShape) {
    case "circle":
        tmp = 0.166 * w;
        return [
            "M", x + w / 2, y,
            "C", x + w + tmp, y, x + w + tmp, y + h, x + w / 2, y + h,
            "C", x - tmp, y + h, x - tmp, y, x + w / 2, y,
            "Z"
        ].join(" ");
    case "diamond":
        return [
            "M", x + w / 2, y,
            "L", x + w, y + h / 2,
            x + w / 2, y + h,
            x, y + h / 2,
            "Z"
        ].join(" ");
    case "square":
        x = Math.round(x + 0.0625 * w);
        y = Math.round(y + 0.0625 * h);
        w = Math.round(0.875 * w);
        h = Math.round(0.875 * h);
        return [
            "M", x, y,
            "L", x + w, y,
            x + w, y + h,
            x, y + h,
            "Z"
        ].join(" ");
    case "triangle":
        return [
            "M", x + w / 2, y,
            "L", x + w, y + h,
            x, y + h,
            "Z"
        ].join(" ");
    case "triangle-down":
        return [
            "M", x, y,
            "L", x + w, y,
            x + w / 2, y + h,
            "Z"
        ].join(" ");
    }
}

function uiErrorShow(err) {
// this function will display <err> to user
    console.error(err);
    document.querySelector("#errorPanel1 .modalContent").textContent = err;
    uiFadeIn(document.querySelector("#errorPanel1"));
}

function uiFadeIn(elem) {
// this function will fade-in <elem>
    elem.style.opacity = (
        elem === UI_CRUD
        ? "1"
        : "0.875"
    );
    elem.style.visibility = "visible";
}

function uiFadeOut(elem) {
// this function will fade-out <elem>
    elem.style.opacity = "0";
    elem.style.visibility = "hidden";
}

async function uiRenderDb() {
// this function will render #dbtableList1
    let dbList;
    // built aside, swapped in with the DOM - a right-click mid-render must
    // still match the old DOM [r16]
    let dbtableDict = new Map();
    let dbtableIi = 0;
    let dbtableListAll;
    let html = "";
    let windowScrollY;
    location.hash = "0";
    windowScrollY = window.scrollY;
    dbList = await dbExecAsync({
        db: DB_MAIN,
        sql: "PRAGMA database_list;"
    });
    dbList = new Set(dbList[0].map(function ({
        name
    }) {
        return `${name}`;
    }));
    for (const db of DB_DICT.values()) {
        const {
            dbName,
            isDbchart,
            isDbquery
        } = db;
        // db detached by sql - forget and close it, else handle leaks in wasm
        if (!isDbchart && !isDbquery && !dbList.has(dbName)) {
            DB_DICT.delete(dbName);
            // REVIEWED r21: unawaited - a rejected close only logs
            dbCloseWhenIdle(db);
        }
    }
    dbtableListAll = await Promise.all(Array.from(
        DB_DICT.values()
    ).map(async function (db) {
        let baton;
        let {
            dbName,
            isDbchart,
            isDbmain,
            isDbquery
        } = db;
        let dbtableList;
        let tmp;
        // detached by user sql - reading it by name threw [r33]
        if (!dbList.has(dbName)) {
            return [db, new Map()];
        }
        dbtableList = noop(
            await dbExecAsync({
                db: DB_MAIN,
                sql: (`
SELECT * FROM ${dbName}.sqlite_schema WHERE type = 'table' ORDER BY tbl_name;
                `)
            })
        )[0];
        if (!dbtableList) {
            return [db, new Map()];
        }
        // chart, query db - hide sqlite internals, e.g. sqlite_sequence left
        // by an AUTOINCREMENT table, which cannot be dropped or charted
        dbtableList = dbtableList.filter(function ({
            tbl_name
        }) {
            return !(
                (isDbchart || isDbquery) &&
                String(tbl_name).startsWith("sqlite_")
            );
        });
        dbtableList = new Map(dbtableList.map(function ({
            sql,
            tbl_name
        }, ii) {
            // WITHOUT ROWID has no rowid - REVIEWED r21: a ")" comment fools it
            const dbtableFullname = `${dbName}.${sqlIdQuote(tbl_name)}`;
            // REVIEWED r26: pragma_table_list.wr untested on missing modules
            const isNoRowid = (/\)[^)]*\bWITHOUT\s+ROWID\b[^)]*$/i).test(sql);
            dbtableIi += 1;
            baton = {
                colList: [],
                colListWritable: [],
                db,
                dbName,
                dbtableFullname,
                dbtableIi,
                dbtableName: sqlIdQuote(tbl_name),
                hashtag: `dbtable_${String(dbtableIi).padStart(2, "0")}`,
                ii,
                isDbchart,
                isDbmain,
                isDbquery,
                rowCount: 0,
                sortCol: 0,
                sortDir: "asc",
                sqlRowid: (
                    isNoRowid
                    ? "NULL AS rowid"
                    // named - an INTEGER PRIMARY KEY id renamed it [r26]
                    : "rowid AS rowid"
                ),
                title: `table ${dbtableFullname}`
            };
            dbtableDict.set(baton.hashtag, baton);
            return [
                baton.dbtableName, baton
            ];
        }));
        // read on DB_MAIN by the name every action uses - TEMP shadows [r28]
        // and re-attached dbs [r33] showed one object, acted on another
        tmp = Array.from(dbtableList.values()).map(function ({
            dbtableFullname,
            dbtableName,
            hashtag
        }) {
            return (`
SELECT '${hashtag}' AS hashtag;
PRAGMA ${dbName}.table_xinfo(${dbtableName});
SELECT COUNT(*) AS rowcount FROM ${dbtableFullname};
            `);
        });
        // one unreadable table, e.g. a virtual table whose module is not
        // compiled in, fails the batch - retry per table, skipping failures
        tmp = await dbExecAsync({
            db: DB_MAIN,
            sql: tmp.join("")
        }).catch(async function () {
            return (await Promise.all(tmp.map(function (sql) {
                return dbExecAsync({
                    db: DB_MAIN,
                    sql
                }).catch(function () {
                    return [];
                });
            }))).flat();
        });
        tmp.forEach(function (rowList) {
            let row0 = rowList[0];
            if (!row0) {
                return;
            }
            if (row0.hasOwnProperty("hashtag")) {
                baton = dbtableDict.get(row0.hashtag);
                return;
            }
            if (row0.hasOwnProperty("rowcount")) {
                baton.rowCount = row0.rowcount;
                return;
            }
            if (row0.hasOwnProperty("cid")) {
                // table_xinfo, not table_info, which skips generated
                // columns that SELECT * returns - headers sat over the
                // wrong cells. hidden 1 is a vtable hidden column, which
                // SELECT * omits; hidden 2, 3 is generated, not writable
                baton.colList = ["rowid"];
                baton.colListWritable = ["rowid"];
                rowList.forEach(function ({
                    hidden,
                    name
                }) {
                    // a column named rowid shadows the real one -
                    // treat it as WITHOUT ROWID [r17]
                    if (String(name).toLowerCase() === "rowid") {
                        baton.sqlRowid = "NULL AS rowid";
                    }
                    if (hidden !== 1) {
                        baton.colList.push(name);
                    }
                    if (!hidden) {
                        baton.colListWritable.push(name);
                    }
                });
            }
        });
        return [db, dbtableList];
    }));
    // set with the swap, so DOM and DBTABLE_DICT come from one call [r18]
    dbtableListAll.forEach(function ([
        db, dbtableList
    ]) {
        db.dbtableList = dbtableList;
    });
    DBTABLE_DICT = dbtableDict;
    // REVIEWED r27: the loop reads the live DB_DICT - a demo attach landing
    // mid-render would throw
    ["#dbchartList1", "#dbqueryList1", "#dbtableList1"].forEach(function (
        selector
    ) {
        const elem = document.querySelector(selector);
        elem.innerHTML = "";
        elem.style.minHeight = "";
    });
    html = "";
    DB_DICT.forEach(function ({
        dbName,
        dbtableList,
        isDbchart,
        isDbquery
    }) {
        // REVIEWED r23: dbtype on the db descriptors couples init to the toc
        const dbtype = (
            isDbchart
            ? "chart"
            : isDbquery
            ? "query"
            : "table"
        );
        html += `<div class="tocTitle">` + (
            isDbchart
            ? "chart"
            : isDbquery
            ? `query result`
            : `database ${dbName}`
        ) + `</div>`;
        dbtableList.forEach(function (baton) {
            const {
                colList,
                dbtableFullname,
                dbtableName,
                hashtag,
                ii,
                rowCount
            } = baton;
            document.querySelector(
                `#db${dbtype}List1`
            ).appendChild(uitableCreate(baton));
            html += `<a class="tocElemA"`;
            html += ` data-hashtag="${hashtag}"`;
            html += ` href="#${hashtag}"`;
            html += ` data-dbtype="${dbtype}"`;
            html += (
                ` title="` +
                stringHtmlSafe((
                    `right-click for crud operation\n\n`
                ) + JSON.stringify({
                    dbtableFullname,
                    rowCount,
                    colList //jslint-ignore-line
                }, undefined, 4)) +
                `"`
            );
            html += `>${ii + 1}. ${dbtype} ${stringHtmlSafe(
                dbtableName.slice(1, -1).replace((/""/g), "\"")
            )}</a>\n`;
        });
    });
    document.querySelector("#tocDbList1").innerHTML = html;
    // restore window.scrollY
    window.scroll({
        behavior: "smooth",
        top: windowScrollY
    });
    uitableInitWithinView({});
}

async function uiTryCatch(func, ...argList) {
// this function will call <func> in a try-catch-block
// that will display any error thrown to user
    try {
        UI_LOADING_COUNTER += 1;
        uiFadeIn(UI_LOADING);
        await func(...argList);
    } catch (err) {
        uiErrorShow(err);
        // false, not undefined - init tells a failed url load by it
        return false;
    } finally {
        // keep the spinner up >= 500ms, but never block the caller on it -
        // awaiting it here made a ?sqlDb= load render 500ms late, as init
        // awaits DB_INIT, which is a uiTryCatch [2026-11-09]
        waitAsync(500).then(function () {
            UI_LOADING_COUNTER -= 1;
            if (
                UI_LOADING_COUNTER === 0 &&
                UI_LOADING.style.visibility === "visible"
            ) {
                uiFadeOut(UI_LOADING);
            }
        });
    }
}

async function uichartCreate(baton) {
// this function will create xy-line-chart from given sqlite table <baton>
    // init pre-var
    let {
        contentElem,
        db,
        dbtableName
    } = baton;
    let elemCanvasFlex;
    let elemLegend;
    // shrunk on a phone by css min-width 200 on .uichartCanvasFlex [r30]
    let elemLegendWidth = 256;
    let elemUichart = contentElem.querySelector(".uichart");
    let elemUichartHeight = 384;
    let uichart = await dbExecAsync({
        db,
        sql: (`
-- table - __chart_options1 - insert
DROP TABLE IF EXISTS temp.__chart_options1;
CREATE TEMP TABLE __chart_options1 AS
    SELECT
        JSON(options) AS options,
        CASTREALORZERO(1.0 / options->>'$.xstep') AS xstep_inv,
        CASTREALORZERO(1.0 / options->>'$.ystep') AS ystep_inv
    FROM (
        SELECT options
        FROM main.${dbtableName}
        WHERE datatype = 'options'
        LIMIT 1
    );

-- table - __chart_series_xy1 - insert
DROP TABLE IF EXISTS temp.__chart_series_xy1;
CREATE TEMP TABLE __chart_series_xy1 AS
    SELECT
        series_index,
        xx,
        yy
    FROM (
        SELECT
            ROW_NUMBER() OVER (
                PARTITION BY series_index, xx ORDER BY rowid DESC
            ) AS rownum,
            series_index,
            xx,
            yy
        FROM (
            SELECT
                ${dbtableName}.rowid,
                --
                series_index,
                IIF(xstep_inv, ROUND(xstep_inv * xx), xx) AS xx,
                IIF(ystep_inv, ROUND(ystep_inv * yy), yy) AS yy
            FROM main.${dbtableName}
            JOIN __chart_options1
            WHERE
                datatype = 'yy_value'
                AND xx IS NOT NULL
        )
    )
    WHERE
        rownum = 1;

-- table - __chart_options1 - select - options
SELECT
        JSON_SET(
            options,
            '$.seriesList', JSON(COALESCE(seriesList, '[]')),
            '$.xdataDxx', COALESCE(xdataDxx, 1),
            '$.xdataMax', xdataMax,
            '$.xdataMin', xdataMin,
            '$.xlabelList', JSON(COALESCE(xlabelList, '[]'))
        ) AS options
    FROM __chart_options1
    JOIN (
        SELECT
            JSON_GROUP_ARRAY(xx_label ORDER BY xx) AS xlabelList
        FROM main.${dbtableName}
        WHERE
            datatype = 'xx_label'
    )
    JOIN (
        SELECT
            JSON_GROUP_ARRAY(JSON(JSON_SET(
                COALESCE(options, '{}'),
                '$.seriesName', series_label,
                '$.xdata', JSON(COALESCE(xdata, '[]')),
                '$.ydata', JSON(COALESCE(ydata, '[]'))
            )) ORDER BY series_index) AS seriesList
        FROM (
            SELECT
                options,
                series_index,
                (
                    ROW_NUMBER() OVER (ORDER BY series_index)
                    || '. '
                    || series_label
                ) AS series_label
            FROM main.${dbtableName}
            WHERE
                datatype = 'series_label'
        )
        -- calculate series.xdata, series.ydata
        LEFT JOIN (
            SELECT
                series_index,
                -- ordered aggregates - GROUP BY order is undocumented [r24]
                JSON_GROUP_ARRAY(xx ORDER BY xx) AS xdata,
                JSON_GROUP_ARRAY(yy ORDER BY xx) AS ydata
            FROM __chart_series_xy1
            GROUP BY series_index
        ) USING (series_index)
    )
    JOIN (
        SELECT
            MAX(xx) AS xdataMax,
            MIN(xx) AS xdataMin
        FROM __chart_series_xy1
        WHERE
            yy IS NOT NULL
    )
    -- calculate uichart.xdataDxx
    JOIN (
        SELECT
            NULLIF(MIN(ABS(xx - xx_prv)), 0) AS xdataDxx
        FROM (
            SELECT
                xx,
                LAG(xx, 1, NULL) OVER (ORDER BY xx) AS xx_prv
            FROM (
                SELECT DISTINCT
                    xx
                FROM __chart_series_xy1
            )
        )
    );
        `)
    });
    // REVIEWED: no options row throws - uiTryCatch shows it
    uichart = JSON.parse(uichart[0][0].options);
    contentElem.querySelector(".uitable").style.display = "none";
    elemUichart.style.display = "flex";
    elemUichart.style.height = `${elemUichartHeight}px`;
    elemUichart.innerHTML = (`
<div
    class="uichartNav"
    style="height: ${elemUichartHeight}px; width: ${elemLegendWidth}px;"
>
    <button
        class="uichartAction"
        data-action="uichartZoomReset"
    >reset zoom</button>
    <button
        class="uichartAction"
        data-action="uichartSeriesHideAll"
    >hide all</button>
    <button
        class="uichartAction"
        data-action="uichartSeriesShowAll"
    >show all</button>
    <div
        class="uichartLegend"
        style="height: ${elemUichartHeight - 64}px;"
    ></div>
</div>
<div style="position: relative; margin-left: 16px; width: 16px;">
    <div class="uichartAxislabel1">${stringHtmlSafe(uichart.yaxisTitle)}</div>
</div>
<div style="display: flex; flex: 1; flex-direction: column; padding: 5px 0;">
    <div class="uichartTitle">
    ${
        stringHtmlSafe(uichart.title).replace((
            /\n/g
        ), "<br>")
    }
    </div>
    <div class="uichartCanvasFlex" style="flex: 1;">
    <svg
        class="uichartCanvasFixed"
        version="1.1"
        xmlns="http://www.w3.org/2000/svg"
    >
        <clipPath class="uichartClip" id="uichartClip${baton.dbtableIi}">
            <rect fill="none" height="0" width="0" x="0" y="0"></rect>
        </clipPath>
        <g class="uichartGridlineList"></g>
        <g class="uichartAxistickList"></g>
        <g class="uichartSeriesList"></g>
        <g class="uichartCrosshairList">
            <path stroke-width="1" stroke="#333" visibility="hidden"></path>
            <path stroke-width="1" stroke="#333" visibility="hidden"></path>
        </g>
        <g class="uichartTooltip" visibility="hidden">
            <rect
                class="uichartTooltipBorder"
                fill-opacity="0.6667"
                fill="#fff"
                rx="5"
                ry="5"
                stroke-width="3"
                x="0"
                y="0"
            >
            </rect>
            <text class="uichartTooltipText"><tspan
                dy="15"
                style="filter: brightness(75%); font-size: 14px;
                    font-weight: normal;"
                x="5"
            ></tspan></text>
        </g>
        <g class="uichartMousetrackerList"></g>
    </svg>
    </div>
    <div class="uichartAxislabel0">${stringHtmlSafe(uichart.xaxisTitle)}</div>
</div>
    `);
    elemCanvasFlex = elemUichart.querySelector(".uichartCanvasFlex");
    elemLegend = elemUichart.querySelector(".uichartLegend");
    let ELEM_GRAPH_LINE_WIDTH = 1; //jslint-ignore-line
    let ELEM_POINT_RADIUS = 4;
    // REVIEWED r15: first draw may include the default 300x150 svg
    let canvasHeight = elemCanvasFlex.clientHeight;
    let canvasWidth = elemCanvasFlex.clientWidth;
    let counterColor = 1;
    let counterShape = 1;
    let elemCanvasFixed = elemCanvasFlex.firstElementChild;
    let [
        elemClip,
        elemGridlineList,
        elemAxistickList,
        elemSeriesList,
        elemCrosshairList,
        elemTooltip,
        elemMousetrackerList
    ] = elemCanvasFixed.children;
    let [
        elemTooltipBorder,
        elemTooltipText
    ] = elemTooltip.children;
    let {
        isBarchart,
        seriesList,
        xdataDxx,
        xdataMax,
        xdataMin,
        xlabelList
    } = uichart;
    let plotHeight;
    let plotLeft;
    let plotTop;
    let plotWidth;
    let plotbarMargin;
    let plotbarYaxis;
    let pointHovered;
    let pointListBarchart = [];
    let redrawTimer;
    let seriesColorList = [
        "#a44", // red
        "#57b", // blue
        "#7ecf6d", // light-green - highcharts-v4
        "#86a", // purple
        "#3d96ae", // aqua - highcharts-v2
        "#f15c80", // pink - highcharts-v4
        "#8085e9", // lavender-blue - highcharts-v4
        "#89a54e", // olive - highcharts-v2
        "#357", // dark-teal
        "#492970" // purple - highcharts-v3
    ];
    let seriesHovered;
    let seriesShapeList = [
        "square",
        "triangle",
        "diamond",
        "triangle-down"
    ];
    let touchState;
    let xaxisMax;
    let xaxisMin;
    let xaxistickDict = {};
    let xpixelToPointDictHovered = [];
    let xtransOffset;
    let xtransWidth;
    let xzoomMax;
    let xzoomMin;
    let yaxisMax;
    let yaxisMin;
    let yaxistickDict = {};
    let ytransWidth;
    function axistickCreate(isXaxis, tickx) {
    // this function will create <elemGridline>, <elemTick>, <elemTicklabel>
    // at given <tickx> in <axis>
        let elemTick;
        let elemTicklabel;
        let elemTspan;
        elemTick = svgAttrSet("path", {
            fill: "none",
            stroke: "#33b",
            "stroke-width": 3
        });
        elemTick.isFirstDraw = true;
        elemTick.isXaxis = isXaxis;
        // if isXaxis, then draw tick
        if (isXaxis) {
            elemAxistickList.appendChild(elemTick);
        } else {
            elemTick.elemGridline = svgAttrSet("path", {
                fill: "none",
                stroke: "#999",
                "stroke-width": 1
            });
            elemGridlineList.appendChild(elemTick.elemGridline);
        }
        // skip ticklabel, if elemTick is not contained in xlabelList
        if (
            isXaxis &&
            xlabelList.length &&
            !xlabelList.hasOwnProperty(tickx - 1)
        ) {
            return elemTick;
        }
        elemTspan = svgAttrSet("tspan");
        elemTspan.textContent = numberFormat({
            modeTick: true,
            num: (
                isXaxis
                ? xlabelList[tickx - 1] ?? tickx
                : tickx
            ),
            xOrY: (
                isXaxis
                ? "x"
                : "y"
            )
        }).slice(0, 16);
        elemTicklabel = svgAttrSet("text", {
            "text-anchor": (
                isXaxis
                ? "middle"
                : "end"
            )
        });
        elemTick.elemTicklabel = elemTicklabel;
        elemTicklabel.fx_rotate = isXaxis;
        elemTicklabel.appendChild(elemTspan);
        elemAxistickList.appendChild(elemTicklabel);
        return elemTick;
    }
    function floatCorrect(num) {
    // this function will correct float-error in <num>
        return parseFloat(
            num.toPrecision(12)
        );
    }
    function graphAnimate(elemGraph, dpath) {
    // this function will draw line <elemGraph> at its final position <dpath>,
    // then animate an svg-transform from the previous axis-scale.
    // svgAnimate cannot tween "d" when the vertex-count changes, which it
    // does on every zoom since offscreen points are dropped, so the line
    // snapped while its dots slid. Each pixel is affine in the axis-scale
    // [ax, bx, ay, by], so tweening the scale moves line and dots together.
        const affineBeg = elemGraph.fx_affine;
        const affineEnd = [
            xtransOffset - xtransWidth * xaxisMin,
            xtransWidth,
            plotHeight + ytransWidth * yaxisMin,
            -ytransWidth
        ];
        const datebeg = UI_ANIMATE_DATENOW;
        function graphAnimateStep() {
            const fxprg = Math.min(1, (
                UI_ANIMATE_DURATION_INV * (UI_ANIMATE_DATENOW - datebeg)
            ));
            let affineNow;
            let sx;
            let sy;
            // superseded by a newer animation of this line - drop from list
            if (graphAnimateStep.isCancelled) {
                return true;
            }
            affineNow = affineBeg.map(function (beg, ii) {
                return beg + fxprg * (affineEnd[ii] - beg);
            });
            sx = affineNow[1] / affineEnd[1];
            sy = affineNow[3] / affineEnd[3];
            elemGraph.fx_affine = affineNow;
            if (fxprg >= 1) {
                elemGraph.removeAttribute("transform");
                return true;
            }
            // map final-position pixel back to current axis-scale
            elemGraph.setAttribute("transform", (
                `matrix(${sx} 0 0 ${sy} ` +
                `${affineNow[0] - sx * affineEnd[0]} ` +
                `${affineNow[2] - sy * affineEnd[2]})`
            ));
            return false;
        }
        elemGraph.setAttribute("d", dpath);
        svgAnimateCancel(elemGraph);
        elemGraph.fx_step = graphAnimateStep;
        // first-draw - nothing to animate from
        if (!affineBeg) {
            elemGraph.fx_affine = affineEnd;
            // a cancelled tween leaves its last matrix behind - clear it
            elemGraph.removeAttribute("transform");
            return;
        }
        graphAnimateStep();
        UI_ANIMATE_LIST.push(graphAnimateStep);
    }
    function numberFormat({
        modeTick,
        num,
        xOrY
    }) {
    // this function will format <num> with axis <xOrY>'s uichart options
        const convert = uichart[xOrY + "valueConvert"];
        const prefix = uichart[xOrY + "valuePrefix"];
        const step = uichart[xOrY + "step"];
        const suffix = uichart[xOrY + "valueSuffix"];
        if (step && typeof num === "number") {
            // 15 significant digits - 3 * 0.1 printed 0.30000000000000004 [r22]
            num = Number(Number(num * step).toPrecision(15));
        }
        switch (convert) {
        case "juliandayToDate":
            num = new Date((num - 2440587.5) * 86400 * 1000);
            num = (
                modeTick
                ? num.toUTCString().slice(5, 16)
                : num.toUTCString().slice(0, 16)
            );
            break;
        case "unixepochToTimelocal":
            num = new Date(num * 1000);
            num = (
                modeTick
                ? num.toLocaleTimeString()
                : num.toLocaleString()
            );
            break;
        case "unixepochToTimeutc":
            num = new Date(num * 1000);
            num = (
                modeTick
                ? num.toUTCString().slice(17)
                : num.toUTCString()
            );
            break;
        }
        if (prefix) {
            num = prefix + num;
        }
        if (suffix) {
            num += suffix;
        }
        return String(num);
    }
    function onCanvasTouch(evt) {
    // this function will handle touchscreen <evt> on canvas-area:
    // 1-finger horizontal drag pans, 1-finger vertical drag scrolls page,
    // 2-finger pinch zooms around the pinch-center
        // every finger on this chart - targetTouches only counts fingers
        // that began on the same svg element, so a pinch from a line and
        // blank plot area read as a jumpy 1-finger pan
        // read once - a read per finger forced a layout per finger per move
        const canvasLeft = elemCanvasFixed.getBoundingClientRect().left;
        const fingerList = Array.from(evt.touches).filter(function ({
            target
        }) {
            return elemCanvasFlex.contains(target);
        }).map(function ({
            clientX,
            clientY
        }) {
            return {
                clientY,
                xfrac: (
                    clientX - canvasLeft - plotLeft
                ) / plotWidth
            };
        });
        let dmin;
        let dspan;
        let spanNow;
        let spanOld;
        // touchstart, touchend - re-anchor gesture to remaining fingers
        if (evt.type !== "touchmove") {
            touchState = fingerList.length && {
                direction: (
                    fingerList.length === 2
                    ? "zoom"
                    : undefined
                ),
                // displayed axis, as fingers see it: plot-pixel 0 is
                // xaxisMin - dxx/2, and the plot spans the padded axis + dxx
                // REVIEWED r21: a lift before the 0 ms redraw jumps one move
                dmin: xaxisMin - 0.5 * xdataDxx,
                dspan: xaxisMax - xaxisMin + xdataDxx,
                fingerList
            };
            return;
        }
        if (!touchState || fingerList.length > 2) {
            return;
        }
        // 1-finger - decide pan or page-scroll from first movement
        if (!touchState.direction) {
            spanNow = plotWidth * Math.abs(
                fingerList[0].xfrac - touchState.fingerList[0].xfrac
            );
            spanOld = Math.abs(
                fingerList[0].clientY - touchState.fingerList[0].clientY
            );
            if (Math.max(spanNow, spanOld) < 8) {
                return;
            }
            touchState.direction = (
                spanNow > spanOld
                ? "pan"
                : "scroll"
            );
        }
        if (touchState.direction === "scroll") {
            return;
        }
        evt.preventDefault();
        dspan = touchState.dspan;
        if (touchState.direction === "pan") {
            dmin = touchState.dmin - dspan * (
                fingerList[0].xfrac - touchState.fingerList[0].xfrac
            );
        } else if (fingerList.length === 2) {
            // REVIEWED r15: aligned fingers clamp to 1%, jitter zooms
            spanOld = Math.max(0.01, Math.abs(
                touchState.fingerList[0].xfrac - touchState.fingerList[1].xfrac
            ));
            spanNow = Math.max(0.01, Math.abs(
                fingerList[0].xfrac - fingerList[1].xfrac
            ));
            // data-x under old pinch-center stays under new pinch-center
            dmin = touchState.dmin + dspan * 0.5 * (
                touchState.fingerList[0].xfrac + touchState.fingerList[1].xfrac
            );
            dspan *= spanOld / spanNow;
            dmin -= dspan * 0.5 * (fingerList[0].xfrac + fingerList[1].xfrac);
        } else {
            return;
        }
        xzoomFromDisplayed(dmin, dspan, (
            touchState.direction === "pan"
            ? 0.5
            : 0.5 * (fingerList[0].xfrac + fingerList[1].xfrac)
        ));
        // uichartRedraw - uichartZoom
        uichartRedraw();
    }
    function onCanvasZoom(evt) {
    // this function will zoom/un-zoom at current mouse-location on canvas-area
        const dspan = xaxisMax - xaxisMin + xdataDxx;
        let xmid = 0.5;
        let xscale = 1.2500; // zoom-out
        // horizontal trackpad swipe - not a zoom, leave it to the page
        if (evt.deltaY === 0) {
            return;
        }
        // a zoom that changes nothing - out at full extent, in at the 4*dxx
        // floor - scrolls the page instead of stalling it [r18, r21]
        if (
            evt.deltaY > 0 &&
            (xzoomMin ?? xdataMin) <= xdataMin &&
            (xzoomMax ?? xdataMax) >= xdataMax
        ) {
            return;
        }
        if (
            evt.deltaY < 0 &&
            (xzoomMax ?? xdataMax) - (xzoomMin ?? xdataMin) <=
            4 * xdataDxx * (1 + 1e-9)
        ) {
            return;
        }
        evt.preventDefault();
        if (!evt.modeDebounce) {
            debounce(
                "onCanvasZoom" + baton.hashtag,
                onCanvasZoom,
                Object.assign(evt, {
                    modeDebounce: true
                })
            );
            return;
        }
        if (evt.deltaY < 0) {
            xmid = (
                evt.clientX -
                elemCanvasFixed.getBoundingClientRect().left -
                plotLeft
            ) / plotWidth;
            xscale = 0.8000; // zoom-in
        }
        // REVIEWED r22: skews the pivot outward, a 2022 zoom feel
        xmid += 0.5 * (xmid - 0.4000);
        // scale the displayed axis about xmid, then invert the padding, so
        // repeated notches no longer drift wider; zoom-out pivots at a fixed
        // 0.55, so in-at-cursor then out returns exactly only at center
        xzoomFromDisplayed(
            xaxisMin - 0.5 * xdataDxx + xmid * (1 - xscale) * dspan,
            xscale * dspan,
            xmid
        );
        // uichartRedraw - uichartZoom
        uichartRedraw();
    }
    function onPointHover(evt) {
    // this function will handle <evt> when mouse hover over point
        let mouseX;
        let pointObj;
        let rect;
        let tooltipBbox;
        let tooltipHeight;
        let tooltipMargin = 32;
        let tooltipWidth;
        let tooltipX;
        let tooltipY;
        rect = elemCanvasFixed.getBoundingClientRect();
        mouseX = Math.round(evt.clientX - rect.left - plotLeft);
        // if mouse is outside canvas-area, then return
        if (!(0 <= mouseX && mouseX <= plotWidth)) {
            return;
        }
        // get closest-series-point to mouse as pointHovered
        pointObj = xpixelToPointDictHovered[mouseX];
        if (
            !pointObj ||
            pointObj === pointHovered ||
            pointObj.series.isHidden
        ) {
            return;
        }
        pointHovered = pointObj;
        onSeriesHover(pointObj.series, true);
        let { //jslint-ignore-line
            pointX,
            pointY,
            xval,
            yval
        } = pointHovered;
        let xlabel;
        let ylabel;
        if (redrawTimer || pointY === undefined) {
            return;
        }
        xlabel = numberFormat({
            num: xlabelList[xval - 1] ?? xval,
            xOrY: "x"
        });
        ylabel = numberFormat({
            num: yval,
            xOrY: "y"
        });
        elemTooltip.setAttribute("visibility", "visible");
        // textContent - no html parse per hover, and no html sink
        elemTooltipText.firstElementChild.textContent = (
            `${ylabel} -- ${xlabel}`
        );
        svgAttrSet(elemTooltipText, {
            fill: seriesHovered.seriesColor
        });
        // update elemTooltipBorder after text-update
        tooltipBbox = elemTooltipText.getBBox();
        tooltipWidth = tooltipBbox.width + 10;
        tooltipHeight = tooltipBbox.height;
        svgAttrSet(elemTooltipBorder, {
            height: tooltipHeight,
            width: tooltipWidth
        });
        tooltipX = pointX + plotLeft - 0.5 * tooltipWidth;
        // left bound last - a tooltip wider than the canvas kept its start
        tooltipX = Math.min(
            tooltipX,
            canvasWidth - tooltipWidth - tooltipMargin
        );
        tooltipX = Math.max(tooltipX, plotLeft + tooltipMargin);
        tooltipY = pointY + plotTop - tooltipHeight - tooltipMargin;
        if (tooltipY < plotTop) {
            tooltipY = pointY + plotTop + tooltipMargin;
        }
        // animate-move tooltip to tooltipX, tooltipY
        svgAnimate(elemTooltip, {
            translateX: tooltipX,
            translateY: tooltipY
        }, "easeout");
        [
            true, false
        ].forEach(function (isXaxis, ii) {
            let d;
            if (isXaxis) {
                d = xaxisTranslate(xval) + CRISPX;
                d = `M ${d} 0 L ${d} ${canvasHeight}`;
            } else {
                d = yaxisTranslate(yval) + CRISPY;
                d = `M 0 ${d} L ${canvasWidth} ${d}`;
            }
            svgAnimate(elemCrosshairList.children[ii], {
                d,
                visibility: "visible"
            }, "easeout");
        });
    }
    function onPointUnhover() {
    // this function will handle <evt> when mouse un-hover from point
        onSeriesUnhover();
        pointHovered = undefined;
        // hide elemcrosshairList, elemTooltip
        elemCrosshairList.children[0].setAttribute("visibility", "hidden");
        elemCrosshairList.children[1].setAttribute("visibility", "hidden");
        elemTooltip.setAttribute("visibility", "hidden");
    }
    function onSeriesHover(evt, scrollTo) {
    // this function will handle <evt> when mouse hover over series
        let series = (
            // a dom event - a series option named target is not one [r20]
            typeof evt.preventDefault === "function"
            ? seriesList[
                evt.target.closest("[data-series-ii]")?.dataset.seriesIi
            ]
            : evt
        );
        let seriesColor;
        if (!series || series.isHidden) {
            onSeriesUnhover();
            return;
        }
        if (series !== seriesHovered) {
            onSeriesUnhover();
        }
        seriesHovered = series;
        // darken series-color - REVIEWED r23: a css filter is untested on ios
        if (isBarchart) {
            seriesColor = series.seriesColor.replace((
                /^#(..)(..)(..)$/
            ), function (ignore, rr, gg, bb) {
                return (
                    "rgb(" +
                    Math.round(0.5 * parseInt(rr, 16)) + "," +
                    Math.round(0.5 * parseInt(gg, 16)) + "," +
                    Math.round(0.5 * parseInt(bb, 16)) +
                    ")"
                );
            });
            seriesHovered.pointListSeries.forEach(function (pointObj) {
                pointObj.elemPoint.setAttribute("fill", seriesColor);
            });
        } else {
            xpixelToPointDictHovered = series.xpixelToPointDict;
            svgAttrSet(seriesHovered.elemGraph, {
                "stroke-width": ELEM_GRAPH_LINE_WIDTH + 2
            });
        }
        // hover series in legend - onSeriesUnhover cleared the old row
        elemLegend.children[series.seriesIi].style.background = "#ccc";
        // REVIEWED r16: offsetTop is from .contentElem - re-derive if moved
        if (scrollTo) {
            debounce("onSeriesScroll" + baton.hashtag, function () {
                elemLegend.scroll({
                    behavior: "smooth",
                    top: (
                        elemLegend.children[series.seriesIi].offsetTop -
                        elemLegend.offsetHeight
                    )
                });
            });
        }
    }
    function onSeriesHoveredHide() {
    // this function will handle <evt> to hide hovered-series
        if (seriesHovered) {
            uichartSeriesHideOrShow(seriesHovered, true);
            uichartRedraw();
        }
    }
    function onSeriesUnhover() {
    // this function will handle <evt> when mouse un-hover from series
        let seriesColor;
        if (!seriesHovered) {
            return;
        }
        if (isBarchart) {
            seriesColor = seriesHovered.seriesColor;
            seriesHovered.pointListSeries.forEach(function (pointObj) {
                pointObj.elemPoint.setAttribute("fill", seriesColor);
            });
        } else {
            svgAttrSet(seriesHovered.elemGraph, {
                "stroke-width": ELEM_GRAPH_LINE_WIDTH
            });
        }
        elemLegend.children[seriesHovered.seriesIi].style.background = "none";
        seriesHovered = undefined;
    }
    async function onUichartAction(evt) {
    // this function will handle uichart event <evt>
        let action;
        let modeHide;
        let target;
        // no stopPropagation - the body click closes the menu [r28]; legend
        // toggles act at once - debounce dropped middle clicks
        if (
            !evt.modeDebounce &&
            !evt.target.closest(`[data-action="uichartSeriesHideOrShow"]`)
        ) {
            debounce(
                "onUichartAction" + baton.hashtag,
                onUichartAction,
                Object.assign(evt, {
                    modeDebounce: true
                })
            );
            return;
        }
        target = evt.target.closest("[data-action]");
        if (!target) {
            return;
        }
        action = target.dataset.action;
        switch (action) {
        case "uichartSeriesHideAll":
        case "uichartSeriesShowAll":
        case "uichartZoomReset":
            uiFadeIn(baton.elemLoading);
            await waitAsync(50);
            if (action === "uichartZoomReset") {
                xzoomMax = undefined;
                xzoomMin = undefined;
            } else {
                modeHide = action === "uichartSeriesHideAll";
                seriesList.forEach(function (series) {
                    uichartSeriesHideOrShow(series, modeHide);
                });
                // each show hovered its series - unhover, else the last one
                // stays thick and highlighted, stuck on touch, no mouseleave
                onSeriesUnhover();
            }
            uichartRedraw();
            await waitAsync(200);
            uiFadeOut(baton.elemLoading);
            return;
        case "uichartSeriesHideOrShow":
            modeHide = target.dataset.hidden !== "1";
            uichartSeriesHideOrShow(
                seriesList[target.dataset.seriesIi],
                modeHide
            );
            uichartRedraw();
            return;
        }
    }
    function pointVisibleSet(pointObj, isVisible) {
    // this function will show or hide marker of <pointObj>,
    // skipping the dom-write if unchanged
    // REVIEWED: a crowded marker vanishes - a fade re-animates hidden ones
        if (Boolean(pointObj.isMarkerHidden) === !isVisible) {
            return;
        }
        pointObj.isMarkerHidden = !isVisible;
        pointObj.elemPoint.setAttribute("visibility", (
            isVisible
            ? "inherit"
            : "hidden"
        ));
    }
    // uichartRedraw - start
    function uichartRedraw(modeDebounce) {
    // this function will redraw <uichart>
        if (!modeDebounce) {
            clearTimeout(redrawTimer);
            redrawTimer = setTimeout(uichartRedraw, 0, true);
            return;
        }
        redrawTimer = undefined;
        // plot margins - 32 bottom and right, plotLeft re-measured from the
        // y labels below
        plotTop = 16;
        plotHeight = canvasHeight - plotTop - 32;
        plotLeft = 32;
        plotWidth = canvasWidth - plotLeft - 32;
        //
        // calculate axisMax, axisMin - start
        //
        [
            true, false
        ].forEach(function (isXaxis) {
            let axisMax;
            let axisMin;
            let tickExponent;
            let tickInterval;
            let tickMultiple;
            let tickx;
            let tickxDict;
            let tickxList;
            let tickxMax;
            let tickxMin;
            let tickxPrv;
            //
            // if isXaxis, then calculate xaxismax, xaxismin
            //
            if (isXaxis) {
                axisMax = Math.min(xdataMax, xzoomMax ?? xdataMax);
                axisMin = Math.max(xdataMin, xzoomMin ?? xdataMin);
                // ensure xaxisMax - xaxisMin >= 4 * xdataDxx
                if (axisMax - axisMin < 4 * xdataDxx) {
                    axisMax = Math.min(xdataMax, axisMin + 4 * xdataDxx);
                    axisMin = Math.max(xdataMin, axisMax - 4 * xdataDxx);
                }
                // pad axisMax, axisMin - xzoomFromDisplayed inverts this and
                // the 1% pad below; change one, change it
                axisMax += 0.02 * (axisMax - axisMin);
                axisMin -= 0.02 * (axisMax - axisMin);
            }
            //
            // if not isXaxis, then calculate yaxisMax, yaxisMin
            //
            // if isBarchart, then make sure yaxis is visible
            if (!isXaxis && isBarchart) {
                axisMax = 0;
                axisMin = 0;
            }
            seriesList.forEach(function (series) {
                const xhi = xaxisMax + 0.5 * xdataDxx;
                const xlo = xaxisMin - 0.5 * xdataDxx;
                let ii;
                let nn;
                let xcropEnd;
                let xcropStart;
                let {
                    xdata,
                    ydata
                } = series;
                let yval;
                if (isXaxis || series.isHidden) {
                    return;
                }
                nn = xdata.length;
                // REVIEWED r19: the crop skips off-range series, 5y zoom
                // crop to [xaxisMin - dxx/2, xaxisMax + dxx/2], so a visible
                // edge point is autoscaled and an invisible one is not
                if (xlo <= xdata[0] && xdata[nn - 1] <= xhi) {
                    xcropEnd = nn;
                    xcropStart = 0;
                // xdata outside xcrop-range
                } else if (xdata[nn - 1] < xlo || xhi < xdata[0]) {
                    xcropEnd = 0;
                    xcropStart = 0;
                } else {
                    ii = 0;
                    while (ii < nn && xdata[ii] < xlo) {
                        ii += 1;
                    }
                    xcropStart = ii;
                    while (ii < nn && xdata[ii] <= xhi) {
                        ii += 1;
                    }
                    xcropEnd = ii;
                }
                // calculate yaxisMax, yaxisMin
                ii = xcropStart;
                while (ii < xcropEnd) {
                    yval = ydata[ii];
                    // skip Infinity too - it made every axis value NaN
                    if (Number.isFinite(yval)) {
                        axisMax = Math.max(axisMax ?? yval, yval);
                        axisMin = Math.min(axisMin ?? yval, yval);
                    }
                    ii += 1;
                }
            });
            axisMax = axisMax ?? 0;
            axisMin = axisMin ?? 0;
            // inverted by xzoomFromDisplayed, with the 2% x pad above
            axisMax += 0.01 * (axisMax - axisMin);
            axisMin -= 0.01 * (axisMax - axisMin);
            if (axisMax === axisMin) {
                axisMax = Math.max(0.99 * axisMax, 1.01 * axisMax);
                axisMin = Math.min(0.99 * axisMin, 1.01 * axisMin);
            }
            if (axisMax === axisMin) {
                axisMax += 1;
                axisMin -= 1;
            }
            //
            // add/remove elemGridline, elemTick, elemTicklabel
            //
            // calculate tickInterval
            tickInterval = (
                isXaxis
                ? (axisMax - axisMin) * 100 / plotWidth
                : (axisMax - axisMin) * 72 / plotHeight
            ) || 1;
            // normalize tickInterval to within 0...10
            tickExponent = Math.pow(
                10,
                Math.floor(Math.log(tickInterval) / Math.LN10)
            );
            tickInterval = tickInterval / tickExponent;
            // round tickInterval to 1, 2, 5, or 10
            Array.from([
                1, 2, 5, 10
            ]).some(function (multiple, ii, list) {
                tickMultiple = multiple;
                return (
                    2 * tickInterval <=
                    tickMultiple + (list[ii + 1] || tickMultiple)
                );
            });
            // after rounding, un-normalize tickInterval from within 0...10
            tickInterval = tickMultiple * tickExponent;
            tickx = floatCorrect(
                Math.floor(axisMin / tickInterval) * tickInterval
            );
            tickxList = [];
            tickxMax = floatCorrect(
                Math.ceil(axisMax / tickInterval) * tickInterval
            );
            while (true) {
                tickxList.push(tickx);
                tickx = floatCorrect(tickx + tickInterval);
                // non-finite axis or plotWidth <= 0 would loop forever
                if (
                    tickx > tickxMax ||
                    tickx === tickxPrv ||
                    !Number.isFinite(tickx) ||
                    tickxList.length > 1000
                ) {
                    break;
                }
                tickxPrv = tickx;
            }
            // sync tickxMax, tickxMin with axisMax, axisMin
            tickxMax = tickxList[tickxList.length - 1];
            tickxMin = tickxList[0];
            if (isXaxis) {
                if (tickxMax > axisMax + 0.5 * xdataDxx) {
                    tickxList.pop();
                }
                if (tickxMin < axisMin - 0.5 * xdataDxx) {
                    tickxList.shift();
                }
            // ticks collapsed by floatCorrect, e.g. 1.7e12 + 0..9, keep the
            // raw axis - else max === min and every pointY is NaN
            } else if (tickxMax > tickxMin) {
                axisMax = tickxMax;
                axisMin = tickxMin;
            }
            // add elemGridline, elemTick, elemTicklabel
            tickxDict = (
                isXaxis
                ? xaxistickDict
                : yaxistickDict
            );
            tickxList.forEach(function (tickx) {
                tickxDict[tickx] = tickxDict[tickx] || axistickCreate(
                    isXaxis,
                    tickx
                );
            });
            // remove elemGridline, elemTick, elemTicklabel
            Object.entries(tickxDict).forEach(function ([
                tickx, elemTick
            ]) {
                if (tickxList.indexOf(Number(tickx)) !== -1) {
                    return;
                }
                elemTick.remove();
                elemTick.elemGridline?.remove();
                elemTick.elemTicklabel?.remove();
                delete tickxDict[tickx];
            });
            //
            // re-calculate plotLeft, plotWidth
            //
            if (!isXaxis) {
                plotLeft = 0;
                tickxList.forEach(function (tickx) {
                    plotLeft = Math.max(
                        plotLeft,
                        tickxDict[tickx].elemTicklabel.getBBox().width
                    );
                });
                plotLeft = Math.round(16 + plotLeft);
                plotWidth = Math.round(canvasWidth - plotLeft - 32);
            }
            // save axisMax, axisMin
            if (isXaxis) {
                xaxisMax = axisMax;
                xaxisMin = axisMin;
            } else {
                yaxisMax = axisMax;
                yaxisMin = axisMin;
            }
        });
        //
        // calculate axisMax, axisMin - end
        //
        //
        // calculate xtransOffset, xtransWidth, ytransWidth
        //
        xtransWidth = plotWidth / (xaxisMax - xaxisMin + xdataDxx);
        xtransOffset = xtransWidth * 0.5 * xdataDxx;
        ytransWidth = plotHeight / (yaxisMax - yaxisMin);
        //
        // calculate plotbarMargin, plotbarWidth, plotbarYaxis
        plotbarMargin = Math.max(2, 0.24 * xtransWidth * xdataDxx);
        plotbarYaxis = yaxisTranslate(0);
        //
        // update crop-area for
        // elemCrosshairList, elemMousetrackerList, elemSeriesList
        //
        // cancel the startup clip tween, else it ends at the old plotWidth
        svgAnimateCancel(elemClip.firstElementChild);
        svgAttrSet(elemClip.firstElementChild, {
            height: plotHeight,
            width: plotWidth
        });
        [
            elemCrosshairList, elemMousetrackerList, elemSeriesList
        ].forEach(function (elem) {
            svgAttrSet(elem, {
                "clip-path": `url(#${elemClip.id})`,
                transform: `translate(${plotLeft},${plotTop})`
            });
        });
        //
        // redraw elemGridline, elemTick, elemTicklabel - start
        //
        [
            Object.entries(xaxistickDict),
            Object.entries(yaxistickDict)
        ].flat().forEach(function ([
            tickx, elemTick
        ]) {
            let {
                elemGridline,
                elemTicklabel,
                isXaxis
            } = elemTick;
            let xx;
            let yy;
            // init xx, yy for first-draw
            if (elemTick.isFirstDraw) {
                xx = plotLeft + 0.5 * plotWidth;
                yy = plotTop + 0.5 * plotHeight;
                if (isXaxis) {
                    yy = canvasHeight + 0.125 * plotHeight;
                } else {
                    xx = -0.125 * plotWidth;
                }
                delete elemTick.isFirstDraw;
                svgAttrSet(elemTick, {
                    d: `M ${xx} ${yy} L ${xx} ${yy + 5}`
                });
                if (elemGridline) {
                    svgAttrSet(elemGridline, {
                        d: (
                            `M ${plotLeft} ${yy}` +
                            ` L ${plotLeft + plotWidth} ${yy}`
                        )
                    });
                }
                if (elemTicklabel) {
                    svgAttrSet(elemTicklabel, {
                        x: xx,
                        y: yy
                    });
                }
            }
            // init xx, yy for animation
            if (isXaxis) {
                xx = plotLeft + xaxisTranslate(tickx);
                yy = plotTop + plotHeight;
            } else {
                xx = plotLeft;
                yy = plotTop + yaxisTranslate(tickx);
            }
            if (isXaxis) {
                svgAnimate(elemTick, {
                    d: `M ${xx - 0.5} ${yy + 0.5} L ${xx - 0.5} ${yy + 5.5}`
                });
            }
            if (elemTicklabel) {
                svgAnimate(elemTicklabel, (
                    isXaxis
                    ? {
                        x: xx,
                        y: yy + 18
                    }
                    : {
                        x: xx - 8,
                        y: yy + 14 * 0.9 - 0.5 * elemTicklabel.getBBox().height
                    }
                ));
            }
            if (elemGridline) {
                svgAnimate(elemGridline, {
                    d: (
                        `M ${plotLeft} ${yy + CRISPY}` +
                        ` L ${plotLeft + plotWidth} ${yy + CRISPY}`
                    )
                });
            }
        });
        //
        // redraw elemGridline, elemTick, elemTicklabel - end
        //
        //
        // redraw seriesList - start
        //
        seriesList.forEach(function (series) {
            // read once - graphAnimate updates it only after this loop
            const affine = series.elemGraph?.fx_affine;
            let {
                elemGraph,
                elemGraphtracker,
                pointListSeries
            } = series;
            let elemGraphD = "";
            let pointXPrv;
            let xviewMax = xaxisMax;
            let xviewMin = xaxisMin;
            if (series.isHidden) {
                return;
            }
            // widen to what the line shows at its pre-tween scale - else the
            // line edge goes blank for the 250ms graphAnimate tween
            if (affine) {
                xviewMax = Math.max(
                    xviewMax,
                    (plotWidth - affine[0]) / affine[1]
                );
                xviewMin = Math.min(xviewMin, -affine[0] / affine[1]);
            }
            // redraw pointListSeries
            // optimization - with many points, e.g. 5 years daily, animating
            // every marker made zoom and pan sluggish (~14k svg paths for 11
            // series), so hide markers that are offscreen or within 8px (x) of
            // the previous shown marker, and skip their animation
            pointListSeries.forEach(function (pointObj, ii) {
                let barY;
                let {
                    elemPoint,
                    xval,
                    yval
                } = pointObj;
                let isOffscreen;
                let pointX;
                let pointY;
                // pixelate xval, yval to pointX, pointY
                pointX = xaxisTranslate(xval);
                // non-finite y (Infinity, NULL) is skipped and the line
                // bridges it - "L x -Infinity" is invalid path data
                pointY = (
                    Number.isFinite(yval)
                    ? yaxisTranslate(yval)
                    : undefined
                );
                // save pointX, pointY
                pointObj.pointX = pointX;
                pointObj.pointY = pointY;
                if (pointY === undefined) {
                    return;
                }
                // offscreen = this point and its neighbor are past one edge;
                // keep the neighbor just past each edge, the line needs it
                // REVIEWED: a NULL edge neighbor gaps only the padding strip
                isOffscreen = (
                    (pointListSeries[ii + 1]?.xval ?? xval) < xviewMin ||
                    xviewMax < (pointListSeries[ii - 1]?.xval ?? xval)
                );
                if (isOffscreen) {
                    pointVisibleSet(pointObj, false);
                    return;
                }
                if (isBarchart) {
                    // if isBarchart, then redraw point as bar
                    barY = Math.min(pointY, plotbarYaxis);
                    // re-shown bar - start at target, not its stale spot
                    if (pointObj.isMarkerHidden) {
                        elemPoint.fx_x = pointX - plotbarMargin;
                        elemPoint.fx_y = barY;
                    }
                    pointVisibleSet(pointObj, true);
                    svgAnimate(elemPoint, {
                        height: Math.max(
                            4,
                            Math.max(pointY, plotbarYaxis) - barY
                        ),
                        width: 2 * plotbarMargin,
                        x: pointX - plotbarMargin,
                        y: barY
                    });
                    return;
                }
                // if not isBarchart, then calculate elemGraphD - every
                // in-range point, so thinning markers never erases a dip
                elemGraphD += (
                    (
                        elemGraphD === ""
                        // moveto
                        ? "M"
                        // lineto
                        : "L"
                    ) +
                    ` ${pointX} ${pointY} `
                );
                // outside the displayed range (axis +- dxx/2) - line only, no
                // marker: the plot clips it,
                // and the widened tween range would unhide thousands of them
                if (
                    xval < xaxisMin - 0.5 * xdataDxx ||
                    xaxisMax + 0.5 * xdataDxx < xval
                ) {
                    pointVisibleSet(pointObj, false);
                    return;
                }
                // if not isBarchart, then hide marker crowding previous one
                if (!(
                    pointXPrv === undefined ||
                    ii + 1 === pointListSeries.length ||
                    // x spacing only - y distance kept noisy series unthinned
                    8 <= pointX - pointXPrv
                )) {
                    pointVisibleSet(pointObj, false);
                    return;
                }
                pointXPrv = pointX;
                // re-shown marker - start where its line is drawn now, not at
                // its stale position, so marker and line move together
                if (pointObj.isMarkerHidden) {
                    elemPoint.fx_x = (
                        affine
                        ? affine[0] + affine[1] * xval
                        : pointX
                    ) - ELEM_POINT_RADIUS;
                    elemPoint.fx_y = (
                        affine
                        ? affine[2] + affine[3] * yval
                        : pointY
                    ) - ELEM_POINT_RADIUS;
                    pointVisibleSet(pointObj, true);
                }
                // if not isBarchart, then redraw point as shape
                svgAnimate(elemPoint, {
                    height: 2 * ELEM_POINT_RADIUS,
                    width: 2 * ELEM_POINT_RADIUS,
                    x: pointX - ELEM_POINT_RADIUS,
                    y: pointY - ELEM_POINT_RADIUS
                });
            });
            // if not isBarchart, then redraw elemGraph
            if (!isBarchart) {
                elemGraphD = elemGraphD.trim();
                graphAnimate(elemGraph, elemGraphD);
                elemGraphtracker.setAttribute("d", elemGraphD);
            }
            series.xpixelToPointDict = xpixelToPointDictCreate(
                pointListSeries
            );
        });
        if (isBarchart) {
            xpixelToPointDictHovered = xpixelToPointDictCreate(
                pointListBarchart
            );
        }
        // point lookup at the new zoom - the hovered series, else the last
        // visible one; unhover restores styles [r16, r17, r23]
        if (seriesHovered?.isHidden) {
            onSeriesUnhover();
        }
        seriesList.forEach(function (series) {
            if (
                !isBarchart &&
                !series.isHidden &&
                (seriesHovered ?? series) === series
            ) {
                xpixelToPointDictHovered = series.xpixelToPointDict;
            }
        });
        // hide tooltip and crosshair - they mark a pre-redraw pixel, and a
        // tap gives no mouseleave; the next mousemove redraws them
        pointHovered = undefined;
        elemCrosshairList.children[0].setAttribute("visibility", "hidden");
        elemCrosshairList.children[1].setAttribute("visibility", "hidden");
        elemTooltip.setAttribute("visibility", "hidden");
        //
        // redraw seriesList - end
        //
    }
    // uichartRedraw - end
    function uichartResize() {
    // this function will resize <uichart>
        // temporarily remove elemCanvasFixed
        // to calculate canvasHeight, canvasWidth
        elemCanvasFixed.remove();
        canvasHeight = Math.round(elemCanvasFlex.clientHeight);
        canvasWidth = Math.round(elemCanvasFlex.clientWidth);
        // restore elemCanvasFixed
        elemCanvasFlex.appendChild(elemCanvasFixed);
        svgAttrSet(elemCanvasFixed, {
            height: canvasHeight,
            width: canvasWidth
        });
        // redraw from uichartResize
        uichartRedraw();
    }
    function uichartSeriesHideOrShow(series, modeHide) {
    // this function will hide-or-show <series>
        if (!series || series.isDummy) {
            return;
        }
        // re-shown line: cancel its tween and drop fx_affine, so it draws
        // fresh at the current zoom; hide every marker via pointVisibleSet
        // while still hidden - a flag-only hide skipped the dom write and
        // left ghost markers at stale pixels
        if (!modeHide && series.isHidden && series.elemGraph) {
            svgAnimateCancel(series.elemGraph);
            delete series.elemGraph.fx_affine;
            for (const pointObj of series.pointListSeries) {
                pointVisibleSet(pointObj, false);
            }
        }
        // reset previously hidden points to yaxis
        if (isBarchart && !modeHide && series.isHidden) {
            series.pointListSeries.forEach(function ({
                elemPoint,
                pointY
            }) {
                if (pointY !== undefined) {
                    svgAttrSet(elemPoint, {
                        height: 0,
                        y: plotbarYaxis
                    });
                }
            });
        }
        series.isHidden = modeHide;
        // REVIEWED r17: a touch legend show stays highlighted till hover
        if (!modeHide) {
            onSeriesHover(series);
        }
        [
            series.elemSeries, series.elemGraphtracker
        ].forEach(function (elem) {
            if (elem) {
                elem.setAttribute("visibility", (
                    modeHide
                    ? "hidden"
                    : "visible"
                ));
            }
        });
        elemLegend.children[series.seriesIi].dataset.hidden = Number(modeHide);
    }
    function xaxisTranslate(xval) {
    // this function will translate <xval> to xpixel position on chart
        return xtransOffset + xtransWidth * (xval - xaxisMin);
    }
    // REVIEWED r17: bars sharing an x map to the last series
    function xpixelToPointDictCreate(pointList) {
    // this function will create dict mapping <xpixel> to nearest point in
    // <pointList> along xaxis
        let dict = [];
        let ii = 0;
        let nn = pointList.length;
        let pointObj;
        let pointObjPrv;
        let xpixel = 0;
        let xpixelEnd;
        while (ii < nn) {
            pointObj = pointList[ii];
            // on-plot points only - edge pixels mapped to an offscreen point
            // with no marker, and its tooltip could land off the svg
            if (
                pointObj.pointY !== undefined &&
                !pointObj.series.isHidden &&
                0 <= pointObj.pointX &&
                pointObj.pointX <= plotWidth
            ) {
                if (pointObjPrv) {
                    // cap at plotWidth - zoomed-in data lies far beyond it
                    xpixelEnd = Math.min(
                        plotWidth,
                        0.5 * (pointObjPrv.pointX + pointObj.pointX)
                    );
                    while (xpixel < xpixelEnd) {
                        dict[xpixel] = pointObjPrv;
                        xpixel += 1;
                    }
                }
                pointObjPrv = pointObj;
            }
            ii += 1;
        }
        while (pointObjPrv && xpixel < plotWidth) {
            dict[xpixel] = pointObjPrv;
            xpixel += 1;
        }
        return dict;
    }
    function xzoomFromDisplayed(dmin, dspan, pivot) {
    // this function will set xzoomMin, xzoomMax so uichartRedraw displays
    // the x-axis from <dmin> spanning <dspan> (plot-pixel 0 to plotWidth).
    // It inverts uichartRedraw's x padding: 2% then 2.04%, then 1% then
    // 1.0404% of that, then dxx/2 each side - so displayed span is
    // 1.06131204 * window + dxx, and its left edge sits
    // 0.03090804 * window + dxx/2 below the window.
    // <pivot> is the plot fraction (pinch center, cursor) that must stay put
    // when the 4*dxx floor clamps the width, else zoom past max slid sideways
        const xwidth = Math.max(
            Math.min(xdataMax - xdataMin, 4 * xdataDxx),
            Math.min(xdataMax - xdataMin, (dspan - xdataDxx) / 1.06131204)
        );
        let xmin;
        dmin += pivot * (dspan - (1.06131204 * xwidth + xdataDxx));
        xmin = dmin + 0.5 * xdataDxx + 0.03090804 * xwidth;
        // clamp window within data
        xmin = Math.max(xdataMin, Math.min(xmin, xdataMax - xwidth));
        xzoomMax = xmin + xwidth;
        xzoomMin = xmin;
        // full extent is "no zoom" - float a + (b - a) can fall short of b,
        // and the full-extent check then ate the page scroll [r20]
        if (xwidth >= xdataMax - xdataMin) {
            xzoomMax = undefined;
            xzoomMin = undefined;
        }
    }
    function yaxisTranslate(yval) {
    // this function will translate <yval> to ypixel position on chart
        return plotHeight - ytransWidth * (yval - yaxisMin);
    }
    //
    // uichartCreate - start
    //
    // Resize the box and re-align all aligned elements
    svgAttrSet(elemCanvasFixed, {
        height: canvasHeight,
        width: canvasWidth
    });
    // REVIEWED r15: tap on touch hides a series - the click feature
    elemCanvasFlex.onclick = onSeriesHoveredHide;
    elemCanvasFlex.onmouseenter = onPointUnhover;
    elemCanvasFlex.onmouseleave = onPointUnhover;
    elemCanvasFlex.onmousemove = onPointHover;
    elemCanvasFlex.onwheel = onCanvasZoom;
    for (const type of [
        "touchcancel", "touchend", "touchmove", "touchstart"
    ]) {
        elemCanvasFlex.addEventListener(type, onCanvasTouch, {
            passive: false
        });
    }
    elemLegend.onmouseleave = onPointUnhover;
    elemLegend.onmouseover = onSeriesHover;
    elemUichart.querySelector(".uichartNav").onclick = onUichartAction;
    seriesList.forEach(function (series, seriesIi) {
        let elemGraph;
        let elemGraphtracker;
        let elemSeries = svgAttrSet("g");
        let {
            isDummy,
            seriesColor,
            seriesShape,
            xdata,
            ydata
        } = series;
        let pointListSeries;
        if (isDummy) {
            seriesColor = "rgba(192,192,192,0)";
        } else if (!(seriesColor && typeof seriesColor === "string")) {
            // a 1-based index picks a color; 0 or other values cycle
            seriesColor = seriesColorList[(
                (
                    (Number.isInteger(seriesColor) && seriesColor > 0)
                    ? seriesColor
                    : counterColor
                ) - 1
            ) % seriesColorList.length];
            counterColor += 1;
        }
        seriesColor = seriesColor.replace((
            /^#(.)(.)(.)$/
        ), "#$1$1$2$2$3$3");
        if (isBarchart || isDummy) {
            seriesShape = "square";
        } else if (!(
            /^(?:circle|diamond|square|triangle|triangle-down)$/
        ).test(seriesShape)) {
            // a name svgShapeDraw knows is kept; a 1-based index picks from
            // seriesShapeList; 0 or an unknown name cycles
            seriesShape = seriesShapeList[(
                (
                    (Number.isInteger(seriesShape) && seriesShape > 0)
                    ? seriesShape
                    : counterShape
                ) - 1
            ) % seriesShapeList.length];
            counterShape += 1;
        }
        // init xdata, ydata
        xdata = new Float64Array(xdata);
        ydata = new Float64Array(ydata.map(function (yval) {
            return yval ?? NaN;
        }));
        if (!isBarchart) {
            elemGraph = svgAttrSet("path", {
                fill: "none",
                stroke: seriesColor,
                "stroke-linecap": "round",
                "stroke-linejoin": "round",
                "stroke-width": ELEM_GRAPH_LINE_WIDTH,
                // keep line-width constant while graphAnimate scales it
                "vector-effect": "non-scaling-stroke"
            });
            elemSeries.appendChild(elemGraph);
            elemGraphtracker = svgAttrSet("path", {
                fill: "none",
                stroke: "rgba(192,192,192,0.0001)",
                "stroke-linecap": "round",
                "stroke-linejoin": "round",
                "stroke-width": ELEM_GRAPH_LINE_WIDTH + 20,
                visibility: "inherit"
            });
            elemGraphtracker.dataset.seriesIi = seriesIi;
            elemGraphtracker.onmouseover = function (evt) {
                onSeriesHover(evt, true);
            };
        }
        pointListSeries = Array.from(ydata).map(function (yval, ii) {
            let elemPoint;
            let pointObj;
            let xval = xdata[ii];
            elemPoint = svgAttrSet((
                isBarchart
                ? "rect"
                : "path"
            ), {
                fill: seriesColor,
                visibility: "inherit",
                y: 0.5 * canvasHeight
            });
            if (!isBarchart) {
                elemPoint.fx_seriesShape = seriesShape;
            }
            pointObj = {
                elemPoint,
                series,
                xval,
                yval
            };
            if (isBarchart) {
                pointListBarchart.push(pointObj);
            }
            return pointObj;
        });
        Object.assign(series, {
            elemGraph,
            elemGraphtracker,
            elemSeries,
            isHidden: series.isDummy || series.isHidden,
            pointListSeries,
            seriesColor,
            seriesIi,
            seriesShape,
            xdata,
            xpixelToPointDict: [],
            ydata
        });
    });
    if (isBarchart) {
        pointListBarchart.sort(function (aa, bb) {
            return aa.xval - bb.xval;
        });
    }
    // draw seriesList in reverse, so first series has highest z-index
    Array.from(seriesList).reverse().forEach(function ({
        elemGraphtracker,
        elemSeries,
        pointListSeries
    }) {
        elemSeriesList.appendChild(elemSeries);
        Array.from(pointListSeries).reverse().forEach(function ({
            elemPoint
        }) {
            elemSeries.appendChild(elemPoint);
        });
        if (!isBarchart) {
            elemMousetrackerList.appendChild(elemGraphtracker);
        }
    });
    elemLegend.innerHTML = uichart.seriesList.map(function (series, ii) {
        return (`
<a
    class="uichartAction uichartLegendElem"
    data-action="uichartSeriesHideOrShow"
    data-dummy="${series.isDummy | 0}"
    data-hidden="${series.isHidden | 0}"
    data-series-ii="${ii}"
    title="${stringHtmlSafe(series.seriesName)}"
>
<svg class="uichartLegendElemSvg" xmlns="http://www.w3.org/2000/svg">
<g>
<path
    d="${svgShapeDraw(series.seriesShape, 0, 2, 10, 10)}"
    fill="${stringHtmlSafe(series.seriesColor)}"
    stroke-width="0"
>
</path>
</g>
</svg>
<span style="margin-left: 12px; position: absolute;"
>${stringHtmlSafe(series.seriesName)}</span>
</a>
        `);
    }).join("");
    // first-draw
    uichartRedraw(true);
    // after first-draw, init startup-animation
    svgAttrSet(elemClip.firstElementChild, {
        width: 0
    });
    svgAnimate(elemClip.firstElementChild, {
        width: plotWidth
    });
    return Object.assign(uichart, {
        uichartResize
    });
    //
    // uichartCreate - end
    //
}

async function uitableAjax(baton, {
    rowList,
    rowOffset,
    type
}) {
    let {
        colList,
        contentElem,
        dbtableName,
        elemInfo,
        elemLoading,
        elemScroller,
        elemTable,
        hashtag,
        isDbchart,
        rowCount,
        sortCol,
        sortDir
    } = baton;
    let html = "";
    // qualify a sort column - "rowid" alone is the NULL AS rowid alias [r18]
    let sortExpr = (
        sortCol === 0
        ? ""
        : dbtableName + "."
    ) + sqlIdQuote(colList[sortCol]);
    let viewRowBeg;
    let viewRowEnd;
    if (baton.rowCount === 0) {
        // uitableLoading - hide
        uiFadeOut(elemLoading);
        return;
    }
    switch (type) {
    // uitableScroll
    case "scroll":
    case "uitableInit":
        if (type === "uitableInit" && isDbchart) {
            await uiTryCatch(async function () {
                // resize uichart
                if (baton.uichart) {
                    baton.uichart.uichartResize();
                    return;
                }
                baton.uichart = await uichartCreate(baton);
            });
            uiFadeOut(elemLoading);
            return;
        }
        // rows map to pixels proportionally, same as table-top below, so
        // the bottom of the scroller reads "to N of N"
        viewRowBeg = Math.max(0, Math.round(
            rowCount * elemScroller.scrollTop /
            Math.max(1, elemScroller.scrollHeight)
        ));
        viewRowEnd = Math.min(rowCount, Math.round(
            rowCount *
            (elemScroller.scrollTop + elemScroller.clientHeight) /
            Math.max(1, elemScroller.scrollHeight)
        ));
        elemInfo.textContent = (
            "showing " +
            new Intl.NumberFormat().format(viewRowBeg + 1) +
            " to " +
            new Intl.NumberFormat().format(viewRowEnd) +
            " of " +
            new Intl.NumberFormat().format(rowCount) +
            " rows"
        );
        // skip expensive table-redraw, if scroll-point is within boundaries
        if (
            contentElem.dataset.init !== "0" &&
            baton.rowOffset <= Math.max(0, viewRowBeg - 1 * UI_VIEW_SIZE) &&
            (
                Math.min(rowCount, viewRowEnd + 1 * UI_VIEW_SIZE) <=
                baton.rowOffset + UI_PAGE_SIZE
            )
        ) {
            return;
        }
        // Do the uitable redraw based on the calculated start point
        baton.rowOffset = Math.max(0, Math.round(
            viewRowBeg + 0.5 * UI_VIEW_SIZE - 0.5 * UI_PAGE_SIZE
        ));
        break;
    }
    // REVIEWED r16: switch(false) skips to draw
    switch (type !== "uitableDraw" && baton.modeAjax) {
    // uitableAjax
    case 0:
        // re-rendered away - querying a dropped table popped an error [r16]
        if (!contentElem.isConnected) {
            return;
        }
        // uitableLoading - show
        uiFadeIn(elemLoading);
        baton.modeAjax = 1;
        // uitable - paginate
        // capture offset - a scroll during the query moves baton.rowOffset,
        // which would mis-position these rows
        rowOffset = baton.rowOffset;
        try {
            rowList = await dbExecAsync({
                db: DB_MAIN,
                responseType: "list",
                sql: (`
SELECT
        ${baton.sqlRowid},
        --
        ${dbtableName}.*
    FROM ${baton.dbtableFullname}
    ORDER BY ${sortExpr} ${sortDir}, rowid
    LIMIT ${Number(UI_PAGE_SIZE)}
    OFFSET ${Number(rowOffset)};
                `)
            });
        } catch (err) {
            // failed - reset modeAjax, show it: scroll, sort do not await [r15]
            baton.modeAjax = 0;
            uiFadeOut(elemLoading);
            // REVIEWED r16: also drops a pending modeAjax 2 scroll
            uiErrorShow(err);
            return;
        }
        rowList = (
            rowList[0]
            ? rowList[0].slice(1)
            : []
        ).map(function (row) {
            return row.map(function (val) {
                return (
                    // bugfix - truncate large text to avoid freezing browser
                    (typeof val === "string" && val.length > 65536)
                    ? val.slice(0, 65536)
                    : val
                );
            });
        });
        // recurse - draw
        await uitableAjax(baton, {
            rowList,
            rowOffset,
            type: "uitableDraw"
        });
        return;
    // debounce
    case 1:
        baton.modeAjax = 2;
        return;
    // debounce
    case 2:
        return;
    }
    // uitableDraw - position the table in the virtual scroller
    // REVIEWED: past ~1M rows the capped scroller height drifts
    elemTable.style.top = Math.max(0, Math.round(
        elemScroller.scrollHeight * rowOffset / (baton.rowCount + 1)
    )) + "px";
    // Insert the required TR nodes into the table for display
    rowList.forEach(function (row) {
        // an integer, or null - a user column named rowid selects NULL [r17]
        const rowid = String(row[0]);
        html += (`
<tr data-dbtype="row" data-hashtag="${hashtag}" data-rowid="${rowid}">
        `);
        row.forEach(function (val) {
            html += "<td>" + stringHtmlSafe(val) + "</td>";
        });
        html += "</tr>";
    });
    elemTable.children[1].innerHTML = html;
    // debounce - throttle
    await waitAsync(500);
    // debounce - next
    if (baton.modeAjax === 2) {
        baton.modeAjax = 0;
        await uitableAjax(baton, {});
        return;
    }
    baton.modeAjax = 0;
    // uitableLoading - hide
    uiFadeOut(elemLoading);
}

function uitableCreate(baton) {
// this function will create a dom-datatable-view of sqlite queries and tables
    let contentElem;
    contentElem = domDivCreate(
        (`
<div class="contentElem" data-init="0" id="${baton.hashtag}">
<div class="contentElemTitle title">${stringHtmlSafe(baton.title)}</div>
<div class="uitableLoading">loading</div>
<div class="uichart" style="display: none;"></div>
<div class="uitable">
    <div class="uitableInfo">showing 0 to 0 of 0 entries</div>
    <div
        class="uitableScroller"
        style="height: ${(UI_VIEW_SIZE + 2) * UI_ROW_HEIGHT}px;"
        tabindex="-1"
    >
        <div
            class="uitableScrollerDummy"
            style="height: ${baton.rowCount * UI_ROW_HEIGHT}px;"
        ></div>
        <table class="uitableTable">
            <thead>
                <tr>
        `) +
        baton.colList.map(stringHtmlSafe).map(function (col, ii) {
            return (
                ii === 0
                ? `<th title="${col}" data-sort="asc">${col}</th>`
                : `<th title="${col}">${col}</th>`
            );
        }).join("") +
        (`
                </tr>
            </thead>
            <tbody>
                <tr data-dbtype="row" data-hashtag="${baton.hashtag}">
                <td colspan="${baton.colList.length}">
                No data available in table
                </td>
                </tr>
            </tbody>
        </table>
    </div>
</div>
</div>
        `)
    ).firstElementChild;
    // init event-handling - sorting
    contentElem.querySelector(
        "thead tr"
    ).onclick = uitableSort.bind(undefined, baton);
    contentElem.querySelector(
        ".uitableScroller"
    ).onscroll = uitableAjax.bind(undefined, baton);
    contentElem.addEventListener("uitableInit", function (evt) {
        uitableAjax(baton, evt);
    });
    Object.assign(baton, {
        contentElem,
        elemInfo: contentElem.querySelector(".uitableInfo"),
        elemLoading: contentElem.querySelector(".uitableLoading"),
        elemScroller: contentElem.querySelector(".uitableScroller"),
        elemTable: contentElem.querySelector(".uitableTable"),
        modeAjax: 0,
        rowOffset: 0
    });
    return contentElem;
}

function uitableInitWithinView({
    modeDebounce
}) {
// this function will defer-init uitables when visible in viewport
    if (!modeDebounce) {
        debounce("uitableInitWithinView", uitableInitWithinView, {
            modeDebounce: true
        });
        return;
    }
    document.querySelectorAll(
        `#contentPanel1 .contentElem[data-init="0"]`
    ).forEach(function (elem) {
        let rect = elem.getBoundingClientRect();
        if (0 <= rect.bottom && rect.top < window.innerHeight) {
            elem.dispatchEvent(new window.CustomEvent("uitableInit"));
            elem.dataset.init = "1";
        }
    });
}

function uitableSort(baton, {
    currentTarget,
    target
}) {
// Function to run on user sort request
    let colIi;
    let direction;
    let elem = target.closest("th");
    if (!elem) {
        return;
    }
    direction = elem.dataset.sort;
    direction = (
        direction === "asc"
        ? "desc"
        : "asc"
    );
    Array.from(currentTarget.children).forEach(function (elemTh, ii) {
        if (elemTh !== elem) {
            elemTh.dataset.sort = "";
            return;
        }
        colIi = ii;
    });
    elem.dataset.sort = direction;
    baton.sortCol = colIi;
    baton.sortDir = direction;
    baton.elemScroller.scrollTop = 0;
    baton.rowOffset = 0;
    uitableAjax(baton, {});
}

function waitAsync(timeout) {
// this function will wait <timeout> milliseconds
    return new Promise(function (resolve) {
        setTimeout(resolve, timeout);
    });
}

// init
// iOS Safari can fire load before this module finishes evaluating
// (top-level await in sqlmath.mjs), so init directly if already loaded.
// uiTryCatch shows any init error, else the page sits on the spinner
if (document.readyState === "complete") {
    uiTryCatch(init);
} else {
    window.addEventListener("load", function () {
        uiTryCatch(init);
    });
}
