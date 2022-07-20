/*jslint beta, bitwise, browser, devel, nomen*/
import {
    assertOrThrow,
    dbCloseAsync,
    dbExecAsync,
    dbFileExportAsync,
    dbFileImportAsync,
    dbOpenAsync,
    debugInline,
    noop,
    sqlmathWebworkerInit
} from "./sqlmath.mjs";
let BLOB_SAVE;
let {
    CodeMirror,
    Highcharts,
    uichartRedraw,
    uichartResize,
    uichartXY
} = globalThis;
let DBTABLE_DICT = new Map();
let DB_CHART;
let DB_DICT = new Map();
let DB_II = 0;
let DB_MAIN;
let DB_QUERY;
let DEBOUNCE_DICT = Object.create(null);
let UI_CHART_HEIGHT = 384;
let UI_CHART_LEGEND_WIDTH = 160;
let UI_CONTEXTMENU = document.getElementById("contextmenu1");
let UI_CONTEXTMENU_BATON;
let UI_CRUD = document.getElementById("crudPanel1");
let UI_EDITOR;
let UI_FILE_OPEN = document.createElement("input");
let UI_FILE_SAVE = document.createElement("a");
let UI_LOADING = document.getElementById("loadingPanel1");
let UI_LOADING_COUNTER = 0;
let UI_PAGE_SIZE = 256;
let UI_ROW_HEIGHT = 16;
let UI_VIEW_SIZE = 20;

noop(
    UI_EDITOR,
    assertOrThrow,
    dbFileExportAsync,
    debugInline
);

async function dbFileAttachAsync({
    db,
    dbData
}) {
// this function will attach database <dbData> to <db>
    let dbAttached;
    let dbName;
    DB_II += 1;
    dbName = `attached_${String(DB_II).padStart(2, "0")}`;
    dbAttached = await dbOpenAsync({
        dbData,
        filename: `file:${dbName}?mode=memory&cache=shared`
    });
    dbAttached.dbName = dbName;
    await dbExecAsync({
        db,
        sql: `ATTACH DATABASE '${dbAttached.filename}' AS ${dbName}`
    });
    DB_DICT.set(dbName, dbAttached);
}

function debounce(key, func, ...argList) {
// this function will debounce <func> with given <key>
    let val = DEBOUNCE_DICT[key];
    if (val) {
        val.func = func;
        return;
    }
    val = {
        func: noop,
        timerTimeout: setTimeout(function () {
            delete DEBOUNCE_DICT[key];
            val.func(...argList);
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
CREATE TABLE __stock_historical(sym TEXT, date TEXT, price FLOAT);
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

DROP TABLE IF EXISTS attached_01.__test3;
CREATE TABLE attached_01.__test3 AS SELECT * FROM __test2;

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
    series_index REAL,
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
            "title":"price vs. date comparison of multiple stocks",
            "xAxisTitle": "date",
            "yAxisTitle": "percent change",
            "yValueSuffix": " %"
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
            *
        FROM (
            SELECT
                series_index,
                series_label
            FROM chart.__stock_chart
            WHERE datatype = 'series_label'
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
    LEFT JOIN __stock_historical
    ON
        sym = series_label
        AND date = xx_label;
UPDATE chart.__stock_chart
    SET
        yy = yy * inv - 1
    FROM (SELECT 1)
    JOIN (
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
    ) USING (series_index);
    `).trim() + "\n");
    // exec demo-sql-query
    await onDbExec({});
    return true;
}

async function demoTradebot() {
// this function will run demo-tradebot
    let val;
    try {
        val = await fetch(".tradebot_public.sqlite");
    } catch (ignore) {
        return;
    }
    val = await val.arrayBuffer();
    await dbFileImportAsync({
        db: DB_MAIN,
        dbData: val
    });
    UI_EDITOR.setValue([
        (`
DROP TABLE IF EXISTS chart._{{ii}}_tradebot_performance_today;
CREATE TABLE chart._{{ii}}_tradebot_performance_today (
    datatype TEXT NOT NULL,
    series_index REAL,
    xx REAL,
    yy REAL,
    series_label REAL,
    xx_label TEXT,
    options TEXT
);
INSERT INTO chart._{{ii}}_tradebot_performance_today (datatype, options)
    SELECT
        'options' AS datatype,
        '{
            "chartType": "column",
            "title": "tradebot performance vs market today",
            "xAxisTitle": "comparisons",
            "yAxisTitle": "percent change",
            "yValueSuffix": " %"
        }' AS options;
INSERT INTO chart._{{ii}}_tradebot_performance_today (
    datatype,
    series_index,
    series_label
)
    SELECT
        'series_label' AS datatype,
        rownum AS series_index,
        acc_name AS series_label
    FROM (
        SELECT
            ROW_NUMBER() OVER (
                ORDER BY
                    acc_name = '__tradebot1' DESC,
                    perc_change DESC
            ) AS rownum,
            acc_name
        FROM tradebot_account
    );
INSERT INTO chart._{{ii}}_tradebot_performance_today (datatype, xx, xx_label)
    SELECT
        'xx_label' AS datatype,
        1 AS xx,
        'stock' AS xx_label;
INSERT INTO chart._{{ii}}_tradebot_performance_today (
    datatype,
    series_index,
    xx,
    yy
)
    SELECT
        'yy_value' AS datatype,
        series_index,
        1 AS xx,
        perc_change AS yy
    FROM (
        SELECT
            series_index,
            series_label
        FROM chart._{{ii}}_tradebot_performance_today
        WHERE datatype = 'series_label'
    )
    LEFT JOIN tradebot_account ON acc_name = series_label;
UPDATE chart._{{ii}}_tradebot_performance_today
    SET
        series_label = printf(
            '%+.2f%% - %s - %s',
            tradebot_account.perc_change,
            series_label,
            company_name
        )
    FROM (SELECT 1)
    LEFT JOIN tradebot_account ON acc_name = series_label
    LEFT JOIN tradebot_stock_basket ON sym = series_label;
        `),
        (`
DROP TABLE IF EXISTS chart._{{ii}}_tradebot_position;
CREATE TABLE chart._{{ii}}_tradebot_position (
    datatype TEXT NOT NULL,
    series_index REAL,
    xx REAL,
    yy REAL,
    series_label REAL,
    xx_label TEXT,
    options TEXT
);
INSERT INTO chart._{{ii}}_tradebot_position (datatype, options)
    SELECT
        'options' AS datatype,
        '{
            "chartType": "column",
            "title": "tradebot positions",
            "xAxisTitle": "asset",
            "yAxisTitle": "percent gain / percent holding",
            "yValueSuffix": " %"
        }' AS options;
INSERT INTO chart._{{ii}}_tradebot_position (
    datatype,
    series_index,
    series_label
)
    SELECT
        'series_label' AS datatype,
        1 AS series_index,
        'percent gain today' AS series_label
    --
    UNION ALL
    --
    SELECT
        'series_label' AS datatype,
        2 AS series_index,
        'percent holding' AS series_label;
INSERT INTO chart._{{ii}}_tradebot_position (datatype, xx, xx_label)
    SELECT
        'xx_label' AS datatype,
        rownum AS xx,
        sym AS xx_label
    FROM (
        SELECT
            ROW_NUMBER() OVER (ORDER BY perc_gain_today DESC) AS rownum,
            sym
        FROM tradebot_position
    );
INSERT INTO chart._{{ii}}_tradebot_position (
    datatype,
    series_index,
    xx,
    yy
)
    SELECT
        'yy_value' AS datatype,
        series_index,
        rownum AS xx,
        IIF(series_index = 1, perc_gain_today, perc_holding) AS yy
    FROM (
        SELECT
            ROW_NUMBER() OVER (ORDER BY perc_gain_today DESC) AS rownum,
            *
        FROM tradebot_position
    )
    JOIN (
        SELECT 1 AS series_index
        UNION ALL
        SELECT 2 AS series_index
    );
        `),
        [
            "7 day",
            "1 month",
            "3 month",
            "6 month",
            "1 year",
            "2 year"
        ].map(function (dateInterval) {
            let tableName = (
                `_{{ii}}_tradebot_historical_`
                + dateInterval.replace(" ", "_")
            );
            return (`
DROP TABLE IF EXISTS chart.${tableName};
CREATE TABLE chart.${tableName} (
    datatype TEXT NOT NULL,
    series_index REAL,
    xx REAL,
    yy REAL,
    series_label REAL,
    xx_label TEXT,
    options TEXT
);
INSERT INTO chart.${tableName} (datatype, options)
    SELECT
        'options' AS datatype,
        '{
            "title":
                "tradebot historical performance vs market - ${dateInterval}",
            "xAxisTitle": "date",
            "yAxisTitle": "percent change",
            "yValueSuffix": " %"
        }' AS options;
INSERT INTO chart.${tableName} (datatype, series_index, series_label)
    SELECT
        'series_label' AS datatype,
        rownum AS series_index,
        sym AS series_label
    FROM (
        SELECT
            ROW_NUMBER() OVER (ORDER BY sym) AS rownum,
            sym
        FROM (SELECT DISTINCT sym FROM tradebot_historical)
        WHERE
            sym IS NOT NULL
    );
INSERT INTO chart.${tableName} (datatype, xx, xx_label)
    SELECT
        'xx_label' AS datatype,
        rownum AS xx,
        ydate AS xx_label
    FROM (
        SELECT
            ROW_NUMBER() OVER (ORDER BY ydate) AS rownum,
            ydate
        FROM (SELECT DISTINCT ydate FROM tradebot_historical)
        WHERE
            ydate >= DATE('NOW', '-${dateInterval}')
    );
INSERT INTO chart.${tableName} (datatype, series_index, xx, yy)
    SELECT
        'yy_value' AS datatype,
        series_index,
        xx,
        price AS yy
    FROM (
        SELECT
            *
        FROM (
            SELECT
                series_index,
                series_label
            FROM chart.${tableName}
            WHERE datatype = 'series_label'
        )
        JOIN (
            SELECT
                xx,
                xx_label
            FROM chart.${tableName}
            WHERE
                datatype = 'xx_label'
        )
    )
    LEFT JOIN tradebot_historical
    ON
        sym = series_label
        AND ydate = xx_label;
UPDATE chart.${tableName}
    SET
        yy = yy * inv - 1
    FROM (SELECT 1)
    JOIN (
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
            FROM chart.${tableName}
            WHERE
                datatype = 'yy_value'
                AND yy > 0
        )
        WHERE
            rownum = 1
    ) USING (series_index);
UPDATE chart.${tableName}
    SET
        series_label = printf(
            '%d. %s - %s',
            series_index,
            series_label,
            company_name
        )
    FROM (SELECT 1)
    LEFT JOIN tradebot_stock_basket ON sym = series_label
    WHERE
        datatype = 'series_label';
            `);
        })
    ].flat().map(function (sql, ii) {
        return sql.trim().replace((
            /\{\{ii\}\}/g
        ), String(ii + 1).padStart(2, "0"));
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
    // cleanup previous blob to prevent memory-leak
    URL.revokeObjectURL(BLOB_SAVE);
    // create new blob
    BLOB_SAVE = URL.createObjectURL(new Blob([
        buf
    ]));
    UI_FILE_SAVE.href = BLOB_SAVE;
    // cleanup blob to prevent memory-leak
    setTimeout(function () {
        URL.revokeObjectURL(UI_FILE_SAVE.href);
    }, 30000);
    UI_FILE_SAVE.download = filename.toLowerCase().replace((
        /[^0-9a-z]+/g
    ), "_").replace((
        /_([^_]+)$/
    ), (
        "_"
        + new Date().toISOString().slice(0, 10).replace((
            /-/g
        ), "")
        + ".$1"
    ));
    UI_FILE_SAVE.click();
}

async function init() {
    let modeDemo = true;
    await sqlmathWebworkerInit({});
    // init DB_XXX
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
            await dbOpenAsync({
                filename: db.filename
            })
        ), db);
        // save db
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
    // init UI_FILE_OPEN
    UI_FILE_OPEN.type = "file";
    // init sqlEditor
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
    // init event-handling
    [
        ["#tocPanel1", "click", onDbAction],
        [".dbExec", "click", onDbExec],
        [".dbcrudExec", "click", onDbcrudExec],
        [".modalCancel", "click", onModalClose],
        [".modalClose", "click", onModalClose],
        ["body", "click", onContextmenu],
        ["body", "contextmenu", onContextmenu],
        [UI_FILE_OPEN, "change", onDbAction],
        [document, "keyup", onKeyUp],
        [window, "hashchange", uitableInitWithinView],
        [window, "resize", onResize],
        [window, "scroll", uitableInitWithinView]
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
    window.scroll(0, 0);
    // init location.search
    await Promise.all(Array.from(
        location.search.slice(1).split("&")
    ).map(async function (elem) {
        let [
            key, val
        ] = elem.split("=");
        switch (key) {
        case "demo":
            switch (val) {
            case "demoDefault":
                modeDemo = undefined;
                await demoDefault();
                return;
            case "demoTradebot":
                modeDemo = undefined;
                await demoTradebot();
                return;
            }
            return;
        case "jsScript":
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
                document.head.appendChild(domDivCreate(`
<style>
#contentPanel1 th {
    max-width: 48px;
}
</style>
                `).firstElementChild);
            }
            return;
        case "sqlDb":
            modeDemo = undefined;
            val = await fetch(val);
            val = await val.arrayBuffer();
            await dbFileImportAsync({
                db: DB_MAIN,
                dbData: val
            });
            return;
        case "sqlScript":
            modeDemo = undefined;
            val = await fetch(val);
            val = await val.text();
            UI_EDITOR.setValue(val);
            return;
        }
    }));
    if (!modeDemo) {
        await uiRenderDb();
        return;
    }
    // init demo
    if (
        await demoTradebot()
    ) {
        return;
    }
    await demoDefault();
}

function jsonHtmlSafe(obj) {
// this function will make <obj> html-safe
// https://stackoverflow.com/questions/7381974
    return JSON.parse(JSON.stringify(obj).replace((
        /&/gu
    ), "&amp;").replace((
        /</gu
    ), "&lt;").replace((
        />/gu
    ), "&gt;"));
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
        // contextmenu - hide
        uiFadeOut(UI_CONTEXTMENU);
        // contextmenu - action
        if (target.closest(".contextmenuElem")) {
            onDbAction(evt);
        }
        return;
    }
    // contextmenu - right-click
    // contextmenu - enable default
    if (ctrlKey || metaKey || shiftKey) {
        return;
    }
    // contextmenu - disable default
    evt.preventDefault();
    evt.stopPropagation();
    // init target
    target = target.closest(`.tocElemA[data-dbtype], tr[data-dbtype="row"]`);
    // contextmenu - hide
    if (!target) {
        uiFadeOut(UI_CONTEXTMENU);
        return;
    }
    // init UI_CONTEXTMENU_BATON
    UI_CONTEXTMENU_BATON = DBTABLE_DICT.get(target.dataset.hashtag) || {};
    baton = UI_CONTEXTMENU_BATON;
    baton.rowid = target.dataset.rowid;
    // show / hide .contextmenuElem
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
            if (target.dataset.rowid === undefined) {
                return;
            }
            break;
        }
        style.display = "block";
    });
    // contextmenu - show
    UI_CONTEXTMENU.children[0].innerHTML = (
        "crud operation for:<br>"
        + stringHtmlSafe(baton.dbtableFullname || "script editor")
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
// this function will open db from file
    let action;
    let baton = UI_CONTEXTMENU_BATON;
    let data;
    let target;
    let title;
    target = evt.target.closest("[data-action]") || evt.target;
    action = target.dataset.action;
    if (!action) {
        return;
    }
    // fast actions that do not require loading
    switch (target !== UI_FILE_OPEN && action) {
    case "dbAttach":
    case "dbOpen":
    case "dbscriptOpen":
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
            `${stringHtmlSafe(baton.dbtableFullname)}<br>${title}`
        );
        UI_CRUD.querySelector("tbody").innerHTML = (
            (`
<tr class="crudInput-new_table" style="display: none;">
    <td><span class="crudLabel">{{new_table}}</span></td>
    <td class="tdInput">
    <input class="crudInput" type="text" value="new_table_1">
    </td>
</tr>
<tr class="crudInput-selected_column" style="display: none;">
    <td><span class="crudLabel">{{selected_column}}</span></td>
    <td class="tdInput">
    <select class="crudInput">
            `)
            + baton.colList.slice(1).map(function (col) {
                return (
                    `<option>${stringHtmlSafe(col)}</option>`
                );
            }).join("")
            + (`
    </select>
    </td>
</tr>
<tr class="crudInput-new_column" style="display: none;">
    <td><span class="crudLabel">{{new_column}}</span></td>
    <td class="tdInput">
    <input class="crudInput" type="text" value="new_column_1">
    </td>
</tr>
            `)
        );
        UI_CRUD.querySelector("textarea").value = String(
            `

-- column - add
ALTER TABLE
    ${baton.dbtableFullname}
ADD
    "{{new_column}}" TEXT NOT NULL DEFAULT '';

-- column - drop
ALTER TABLE
    ${baton.dbtableFullname}
DROP COLUMN
    "{{selected_column}}";

-- column - rename
ALTER TABLE
    ${baton.dbtableFullname}
RENAME
    "{{selected_column}}"
TO
    "{{new_column}}";

-- row - insert
INSERT INTO ${baton.dbtableFullname} (`
            + JSON.stringify(baton.colList.slice(1), undefined, 4).slice(1, -1)
            + `) VALUES (\n`
            + `${"    NULL,\n".repeat(baton.colList.length - 2)}    NULL`
            + `
);

-- row - update
UPDATE
    ${baton.dbtableFullname}
SET
`
            + baton.colList.slice(1).map(function (col) {
                return `    "${col}" = NULL`;
            }).join(",\n")
            + `
WHERE
    rowid = ${baton.rowid};

-- table - rename
ALTER TABLE
    ${baton.dbtableFullname}
RENAME TO
    "{{new_table}}";
            `
        ).trim().split("\n\n").filter(function (sql) {
            return sql.indexOf(title) === 3;
        })[0] + "\n";
        switch (action) {
        case "dbcolumnAdd":
            UI_CRUD.querySelectorAll(
                ".crudInput-new_column"
            ).forEach(function (elem) {
                elem.style.display = "table-row";
            });
            break;
        case "dbcolumnDrop":
            UI_CRUD.querySelectorAll(
                ".crudInput-selected_column"
            ).forEach(function (elem) {
                elem.style.display = "table-row";
            });
            break;
        case "dbcolumnRename":
            UI_CRUD.querySelectorAll(
                ".crudInput-new_column, .crudInput-selected_column"
            ).forEach(function (elem) {
                elem.style.display = "table-row";
            });
            break;
        case "dbtableRename":
            UI_CRUD.querySelectorAll(
                ".crudInput-new_table"
            ).forEach(function (elem) {
                elem.style.display = "table-row";
            });
            break;
        }
        uiFadeIn(UI_CRUD);
        return;
    }
    // slow actions that require loading
    if (!evt.modeTryCatch) {
        evt.modeTryCatch = true;
        await uiTryCatch(onDbAction, evt);
        return;
    }
    evt.preventDefault();
    evt.stopPropagation();
    switch (target === UI_FILE_OPEN && action) {
    case "dbAttach":
        if (target.files.length === 0) {
            return;
        }
        await dbFileAttachAsync({
            db: DB_MAIN,
            dbData: (
                await target.files[0].arrayBuffer()
            )
        });
        await uiRenderDb();
        return;
    case "dbOpen":
        if (target.files.length === 0) {
            return;
        }
        await dbFileImportAsync({
            db: DB_MAIN,
            dbData: (
                await target.files[0].arrayBuffer()
            )
        });
        await uiRenderDb();
        return;
    case "dbscriptOpen":
        UI_EDITOR.setValue(
            await target.files[0].text()
        );
        await uiRenderDb();
        return;
    }
    switch (action) {
    case "dbDetach":
        if (!window.confirm(
            "are you sure you want to detach and close database"
            + ` ${baton.dbName} ?`
        )) {
            return;
        }
        await dbExecAsync({
            db: DB_MAIN,
            sql: `DETACH ${baton.dbName};`
        });
        await dbCloseAsync({
            db: baton.db
        });
        await uiRenderDb();
        return;
    case "dbExec":
        await onDbExec({});
        return;
    case "dbExport":
        data = await dbFileExportAsync({
            db: baton.db
        });
        data = data[6];
        fileSave({
            buf: data,
            filename: `sqlite_database_${baton.dbName}.sqlite`
        });
        return;
    case "dbExportMain":
        data = await dbFileExportAsync({
            db: DB_MAIN
        });
        data = data[6];
        fileSave({
            buf: data,
            filename: `sqlite_database_${DB_MAIN.dbName}.sqlite`
        });
        return;
    case "dbRefresh":
        await uiRenderDb();
        return;
    case "dbrowDelete":
        if (!window.confirm(
            `are you sure you want to delete row with rowid = ${baton.rowid}`
            + ` in table ${baton.dbtableFullname} ?`
        )) {
            return;
        }
        await dbExecAsync({
            db: baton.db,
            sql: (`
DELETE FROM ${baton.dbtableName} WHERE rowid = ${baton.rowid};
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
            db: baton.db,
            sql: `DROP TABLE ${baton.dbtableName};`
        });
        await uiRenderDb();
        return;
    case "dbtableSaveCsv":
        data = await dbExecAsync({
            db: baton.db,
            responseType: "list",
            sql: `SELECT rowid, * FROM ${baton.dbtableName};`
        });
        data = data[0] || [];
        data.shift();
        data = rowListToCsv({
            colList: baton.colList,
            rowList: data
        });
        fileSave({
            buf: data || "",
            filename: `sqlite_table_${baton.dbtableName}.csv`
        });
        return;
    case "dbtableSaveJson":
        data = await dbExecAsync({
            db: baton.db,
            sql: `SELECT rowid, * FROM ${baton.dbtableName};`
        });
        data = JSON.stringify(data[0] || []);
        fileSave({
            buf: data,
            filename: `sqlite_table_${baton.dbtableName}.json`
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
        await uiTryCatch(onDbExec, {
            modeTryCatch: true
        });
        return;
    }
    // close error modal
    uiFadeOut(document.querySelector("#errorPanel1"));
    // DBTABLE_DICT - cleanup old uitable
    DBTABLE_DICT.forEach(function ({
        isDbchart,
        isDbquery
    }, key) {
        if (isDbchart || isDbquery) {
            DBTABLE_DICT.delete(key);
        }
    });
    await Promise.all([
        DB_CHART, DB_QUERY
    ].map(async function (db) {
        let sqlCleanup = noop(
            await dbExecAsync({
                db,
                sql: (`
BEGIN TRANSACTION;
SELECT
        group_concat('DROP TABLE [' || name || '];', '') AS sql
    FROM sqlite_master WHERE type = 'table';
END TRANSACTION;
                `)
            })
        )[0][0].sql || "";
        await dbExecAsync({
            db,
            sql: sqlCleanup
        });
    }));
    // 1. exec sql-command in webworker
    dbqueryList = await dbExecAsync({
        db: DB_MAIN,
        responseType: "list",
        sql: UI_EDITOR.getValue()
    });
    // 2. save query-result
    await Promise.all(dbqueryList.map(async function (rowList, ii) {
        let colList = rowList.shift().map(function (col, ii) {
            return `value->>${ii} AS [${col}]`;
        }).join(",");
        await dbExecAsync({
            bindList: {
                tmp1: JSON.stringify(rowList)
            },
            db: DB_QUERY,
            sql: (`
BEGIN TRANSACTION;
CREATE TABLE result_${String(ii).padStart(2, "0")} AS
    SELECT
        ${colList}
    FROM json_each($tmp1);
END TRANSACTION;
            `)
        });
    }));
    // 3. ui-render sql-queries to html
    await uiRenderDb();
}

async function onDbcrudExec({
    modeTryCatch
}) {
// this function will exec crud operation
    let sql;
    if (!modeTryCatch) {
        await uiTryCatch(onDbcrudExec, {
            modeTryCatch: true
        });
        return;
    }
    sql = document.querySelector("#crudPanel1 textarea").value.replace((
        /\{\{\w+?\}\}/g
    ), function (match0) {
        let val = document.querySelector(
            `#crudPanel1 .crudInput-${match0.slice(2, -2)} .crudInput`
        );
        if (!val) {
            return match0;
        }
        if (val.tagName === "SELECT") {
            return val.selectedOptions[0].textContent;
        }
        return val.value.trim();
    });
    await dbExecAsync({
        db: DB_MAIN,
        sql
    });
    await uiRenderDb();
    uiFadeOut(UI_CRUD);
}

function onKeyUp(evt) {
// this function will handle event keyup
    if (!evt.modeDebounce) {
        debounce("onKeyUp", onKeyUp, Object.assign(evt, {
            modeDebounce: true
        }));
        return;
    }
    switch (evt.key) {
    case "Escape":
        // close error-modal
        uiFadeOut(document.querySelector("#errorPanel1"));
        // close contextmenu
        uiFadeOut(UI_CONTEXTMENU);
        return;
    }
    switch ((evt.ctrlKey || evt.metaKey) && evt.key) {
    case "Enter":
        onDbExec({});
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
    document.querySelectorAll(
        "#dbchartList1 .contentElem"
    ).forEach(function (elem) {
        elem.dataset.init = "0";
    });
    uitableInitWithinView({});
}

async function onUichartAction(evt) {
// this function will handle uichart event <evt>
    let action;
    let baton;
    let data;
    let series;
    let target;
    let uichart;
    let xAxis;
    evt.preventDefault();
    evt.stopPropagation();
    if (!evt.modeDebounce) {
        debounce("onUichartAction", onUichartAction, Object.assign(evt, {
            modeDebounce: true
        }));
        return;
    }
    target = evt.target.closest("[data-action]");
    if (!target) {
        return;
    }
    action = target.dataset.action;
    baton = DBTABLE_DICT.get(
        target.closest("#dbchartList1 .contentElem").id
    );
    uichart = baton.uichart;
    xAxis = uichart.axisList[0];
    switch (action) {
    case "uichartSeriesHideAll":
    case "uichartSeriesShowAll":
        uiFadeIn(baton.elemLoading);
        await waitAsync(50);
        data = action === "uichartSeriesShowAll";
        // hide or show legend
        target.parentElement.querySelectorAll(
            ".uichartLegendElem"
        ).forEach(function (elem) {
            elem.dataset.hidden = Number(!data);
        });
        // hide or show series
        uichart.seriesList.forEach(function (series) {
            series.setVisible(data);
        });
        await waitAsync(200);
        uiFadeOut(baton.elemLoading);
        return;
    case "uichartSeriesHideOrShow":
        series = uichart.seriesList[target.dataset.ii];
        data = target.dataset.hidden === "1";
        // hide or show legend
        target.dataset.hidden = Number(!data);
        // hide or show series
        series.setVisible(data);
        return;
    case "uichartZoom":
        (function () {
            let {
                dataMax,
                dataMin,
                max: xMax,
                min: xMin
            } = uichart.axisList[0];
            let xMid = (
                xAxis.min
                + (uichartXY(uichart, evt)[0] / uichart.plotWidth)
                * (xMax - xMin)
            );
            let xNewMax;
            let xNewMin;
            let xScale = (
                evt.deltaY < 0
                ? 0.6667
                : 1.75
            );
            xMid = Math.max(xMin, Math.min(xMax, xMid));
            xNewMax = Math.min(dataMax, xMid + xScale * (xMax - xMid));
            xNewMin = Math.max(dataMin, xMid + xScale * (xMin - xMid));
            xAxis.userMin = xNewMin;
            xAxis.userMax = xNewMax;
            // uichartRedraw - uichartZoom
            uichartRedraw(uichart);
        }());
        return;
    case "uichartZoomReset":
        uiFadeIn(baton.elemLoading);
        await waitAsync(50);
        delete xAxis.userMin;
        delete xAxis.userMax;
        // uichartRedraw - uichartZoomReset
        uichartRedraw(uichart);
        await waitAsync(200);
        uiFadeOut(baton.elemLoading);
        return;
    }
}

function rowListToCsv({
    colList,
    rowList
}) {
// this function will convert json <rowList> to csv with given <colList>
    let data = JSON.stringify([[colList], rowList].flat(), undefined, 1);
    // convert data to csv
    data = data.replace((
        /\n  /g
    ), "");
    data = data.replace((
        /\n \[/g
    ), "");
    data = data.replace((
        /\n \],?/g
    ), "\r\n");
    data = data.slice(1, -2);
    // sqlite-strings are c-strings which should never contain null-char
    data = data.replace((
        /\u0000/g
    ), "");
    // hide double-backslash `\\\\` as null-char
    data = data.replace((
        /\\\\/g
    ), "\u0000");
    // escape double-quote `\\"` to `""`
    data = data.replace((
        /\\"/g
    ), "\"\"");
    // replace newline with space
    data = data.replace((
        /\\r\\n|\\r|\\n/g
    ), " ");
    // restore double-backslash `\\\\` from null-char
    data = data.replace((
        /\u0000/g
    ), "\\\\");
    return data;
}

function stringHtmlSafe(str) {
// this function will make <str> html-safe
// https://stackoverflow.com/questions/7381974
    if (typeof str !== "string") {
        str = String(str);
    }
    return str.replace((
        /&/gu
    ), "&amp;").replace((
        /</gu
    ), "&lt;").replace((
        />/gu
    ), "&gt;").replace((
        /"/gu
    ), "&quot;");
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
    let dbtableIi = 0;
    let html = "";
    let windowScrollY;
    // reset location.hash
    location.hash = "0";
    // save window.scrollY
    windowScrollY = window.scrollY;
    // DB_DICT - sync
    dbList = await dbExecAsync({
        db: DB_MAIN,
        sql: "PRAGMA database_list;"
    });
    dbList = new Set(dbList[0].map(function ({
        name
    }) {
        return `${name}`;
    }));
    DB_DICT.forEach(function ({
        dbName,
        isDbchart,
        isDbquery
    }) {
        if (!isDbchart && !isDbquery && !dbList.has(dbName)) {
            DB_DICT.delete(dbName);
        }
    });
    // DBTABLE_DICT - cleanup old uitable
    DBTABLE_DICT.forEach(function ({
        isDbchart,
        isDbquery
    }, key) {
        if (!isDbchart && !isDbquery) {
            DBTABLE_DICT.delete(key);
        }
    });
    // DBTABLE_DICT - sync
    await Promise.all(Array.from(
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
        db.dbtableList = [];
        dbtableList = noop(
            await dbExecAsync({
                db,
                sql: (`
SELECT * FROM sqlite_schema WHERE type = 'table' ORDER BY tbl_name;
                `)
            })
        )[0];
        if (!dbtableList) {
            return;
        }
        dbtableList = dbtableList.map(function ({
            colList = [],
            dbtableFullname,
            rowCount = 0,
            rowList0,
            sql,
            tbl_name
        }) {
            dbtableFullname = dbtableFullname || `${dbName}.[${tbl_name}]`;
            dbtableIi += 1;
            baton = {
                colList,
                db,
                dbName,
                dbtableFullname,
                dbtableName: `[${tbl_name}]`,
                hashtag: `dbtable_${String(dbtableIi).padStart(2, "0")}`,
                isDbchart,
                isDbmain,
                isDbquery,
                rowCount,
                rowList0,
                sortCol: 0,
                sortDir: "asc",
                sql,
                title: `table ${dbtableFullname}`
            };
            DBTABLE_DICT.set(baton.hashtag, baton);
            return baton;
        });
        tmp = "";
        dbtableList.forEach(function ({
            dbtableName,
            hashtag
        }) {
            tmp += (`
SELECT '${hashtag}' AS hashtag;
PRAGMA table_info(${dbtableName});
SELECT COUNT(*) AS rowcount FROM ${dbtableName};
            `);
        });
        tmp = await dbExecAsync({
            db,
            sql: tmp
        });
        tmp.forEach(function (rowList) {
            let row0 = rowList[0];
            if (!row0) {
                return;
            }
            [
                "cid", "hashtag", "rowcount"
            ].forEach(function (key) {
                switch (row0.hasOwnProperty(key) && key) {
                case "cid":
                    baton.colList = [
                        "rowid",
                        rowList.map(function ({
                            name
                        }) {
                            return name;
                        })
                    ].flat();
                    break;
                case "hashtag":
                    baton = DBTABLE_DICT.get(row0.hashtag);
                    break;
                case "rowcount":
                    baton.rowCount = row0.rowcount;
                    break;
                }
            });
        });
        db.dbtableList = dbtableList;
    }));
    // ui-render databases and tables to html
    document.querySelector("#dbchartList1").innerHTML = "";
    document.querySelector("#dbqueryList1").innerHTML = "";
    document.querySelector("#dbtableList1").innerHTML = "";
    DB_DICT.forEach(function ({
        dbtableList,
        isDbchart,
        isDbquery
    }) {
        dbtableList.forEach(function (baton) {
            // create uitable
            document.querySelector(
                isDbchart
                ? "#dbchartList1"
                : isDbquery
                ? "#dbqueryList1"
                : "#dbtableList1"
            ).appendChild(uitableCreate(baton));
        });
    });
    // ui-render #tocPanel1
    html = "";
    DB_DICT.forEach(function ({
        dbName,
        dbtableList,
        isDbchart,
        isDbquery
    }) {
        html += `<div class="tocTitle">` + (
            isDbchart
            ? "chart"
            : isDbquery
            ? `query result`
            : `database ${dbName}`
        ) + `</div>`;
        dbtableList.forEach(function ({
            colList,
            dbtableFullname,
            dbtableName,
            hashtag,
            rowCount
        }, ii) {
            html += `<a class="tocElemA"`;
            html += ` data-hashtag="${hashtag}"`;
            html += ` href="#${hashtag}"`;
            html += (
                isDbchart
                ? ` data-dbtype="chart"`
                : isDbquery
                ? ` data-dbtype="query"`
                : ` data-dbtype="table"`
            );
            html += (
                ` title="`
                + stringHtmlSafe((
                    `right-click for crud operation\n\n`
                ) + JSON.stringify({
                    colList,
                    dbtableFullname,
                    rowCount
                }, undefined, 4))
                + `"`
            );
            html += `>${ii + 1}. `;
            html += (
                isDbchart
                ? "chart"
                : isDbquery
                ? "query"
                : "table"
            );
            html += ` ${stringHtmlSafe(dbtableName.slice(1, -1))}</a>\n`;
        });
    });
    document.querySelector("#tocDbList1").innerHTML = html;
    // restore window.scrollY
    window.scroll(0, windowScrollY);
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
        console.error(err);
        document.querySelector(
            "#errorPanel1 .modalContent"
        ).textContent = err;
        uiFadeIn(document.querySelector(
            "#errorPanel1"
        ));
    } finally {
        await waitAsync(500);
        UI_LOADING_COUNTER -= 1;
        if (
            UI_LOADING_COUNTER === 0
            && UI_LOADING.style.visibility === "visible"
        ) {
            uiFadeOut(UI_LOADING);
        }
    }
}

async function uichartCreate(baton) {
// this function will create xy-line-chart from given sqlite table <baton>
    let {
        contentElem,
        db,
        dbtableName,
        uichart
    } = baton;
    let options;
    // resize uichart
    if (uichart) {
        uichartResize(uichart);
        return;
    }
    options = await dbExecAsync({
        db,
        sql: (`
SELECT
        json_insert(
            json_insert(options, '$.xxList', json(xxList)),
            '$.seriesList',
            json(seriesList)
        ) AS options
    FROM (SELECT options FROM ${dbtableName} WHERE datatype = 'options' LIMIT 1)
    JOIN (
        SELECT
            json_group_array(xx_label) AS xxList
        FROM ${dbtableName}
        WHERE
            datatype = 'xx_label'
    )
    JOIN (
        SELECT
            json_group_array(
                json_object(
                    'data', json(data),
                    'name', series_label
                )
            ) AS seriesList
        FROM (
            SELECT
                series_index,
                series_label
            FROM ${dbtableName}
            WHERE
                datatype = 'series_label'
        )
        JOIN (
            SELECT
                series_index,
                json_group_array(yy) AS data
            FROM ${dbtableName}
            WHERE
                datatype = 'yy_value'
            GROUP BY
                series_index
            ORDER BY
                series_index,
                xx
        ) USING (series_index)
    );
        `)
    });
    options = JSON.parse(options[0][0].options);
    contentElem.querySelector(".uitable").style.display = "none";
    contentElem.querySelector(".uichart").style.display = "flex";
    contentElem.querySelector(
        ".uichartAxis0Label"
    ).textContent = options.xAxisTitle;
    contentElem.querySelector(
        ".uichartAxis1Label"
    ).textContent = options.yAxisTitle;
    contentElem.querySelector(
        ".uichartTitle"
    ).textContent = options.title;
    options = {
        chart: {
            renderTo: contentElem.querySelector(".uichartCanvas"),
            type: options.chartType || "line"
        },
        seriesList: options.seriesList,
        xAxis: {
            categories: options.xxList,
            labels: {
                rotation: -15
            }
        },
        yAxis: {},
        yValueSuffix: options.yValueSuffix
    };
    uichart = new Highcharts.Chart(options);
    baton.uichart = uichart;
    contentElem.querySelector(".uichart").chart = uichart;
    // init .uichartLegend
    contentElem.querySelector(
        ".uichartLegend"
    ).innerHTML = uichart.seriesList.map(function (series, ii) {
        return (`
<a
    class="uichartAction uichartLegendElem"
    data-action="uichartSeriesHideOrShow"
    data-hidden="0"
    data-ii="${ii}"
    title="${stringHtmlSafe(series.name)}"
>
<svg class="uichartLegendElemSvg" xmlns="http://www.w3.org/2000/svg">
<g>
<path
    d="M 0 8 L 16 8"
    fill="none"
    stroke-width="2"
    style="stroke: ${series.color};"
>
</path>
<path
    d="
        ${
            uichart.renderer.symbols[
                series.symbol || "square"
            ](4, 4, 8, 8).join(" ")
        }
    "
    fill="none"
    style="stroke: ${series.color}; fill: ${series.color};"
>
</path>
</g>
</svg>
<span style="margin-left: 24px; position: absolute;">${series.name}</span>
</a>
        `);
    }).join("");
    // init event-handling
    contentElem.querySelector(".uichartNav").onclick = onUichartAction;
    contentElem.querySelector(".uichartCanvas").onwheel = onUichartAction;
}

async function uitableAjax(baton, {
    rowList,
    type
}) {
    let {
        colList,
        contentElem,
        db,
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
            uiFadeIn(elemLoading);
            await uiTryCatch(uichartCreate, baton);
            await waitAsync(500);
            uiFadeOut(elemLoading);
            return;
        }
        viewRowBeg = Math.max(0, Math.round(
            rowCount
            * elemScroller.scrollTop
            / (elemScroller.scrollHeight - 1 * UI_ROW_HEIGHT)
        ));
        viewRowEnd = Math.min(rowCount, Math.round(viewRowBeg + UI_VIEW_SIZE));
        // update table-view info
        elemInfo.textContent = (
            "showing "
            + new Intl.NumberFormat().format(viewRowBeg + 1)
            + " to "
            + new Intl.NumberFormat().format(viewRowEnd)
            + " of "
            + new Intl.NumberFormat().format(rowCount)
            + " rows"
        );
        // skip expensive table-redraw, if scroll-point is within boundaries
        if (
            contentElem.dataset.init !== "0"
            && baton.rowOffset <= Math.max(0, viewRowBeg - 1 * UI_VIEW_SIZE)
            && (
                Math.min(rowCount, viewRowEnd + 1 * UI_VIEW_SIZE)
                <= baton.rowOffset + UI_PAGE_SIZE
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
    switch (type !== "uitableDraw" && baton.modeAjax) {
    // uitableAjax
    case 0:
        // uitableLoading - show
        uiFadeIn(elemLoading);
        baton.modeAjax = 1;
        // uitable - paginate
        rowList = await dbExecAsync({
            db,
            responseType: "list",
            sql: (`
SELECT
        rowid,
        *
    FROM ${dbtableName}
    ORDER BY [${colList[sortCol]}] ${sortDir}
    LIMIT ${Number(UI_PAGE_SIZE)}
    OFFSET ${Number(baton.rowOffset)};
            `)
        });
        rowList = (
            rowList[0]
            ? rowList[0].slice(1)
            : []
        );
        // recurse - draw
        await uitableAjax(baton, {
            rowList,
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
    // uitableDraw
    // Position the table in the virtual scroller
    elemTable.style.top = Math.max(0, Math.round(
        elemScroller.scrollHeight * baton.rowOffset / (baton.rowCount + 1)
    )) + "px";
    // Insert the required TR nodes into the table for display
    jsonHtmlSafe(rowList).forEach(function (row) {
        html += (`
<tr data-dbtype="row" data-hashtag="${hashtag}" data-rowid="${row[0]}">
        `);
        row.forEach(function (val) {
            html += "<td>" + (val ?? "") + "</td>";
        });
        html += "</tr>";
    });
    elemTable.children[1].innerHTML = html;
    // debounce - throttle
    await waitAsync(500);
    // debounce - next
    if (baton.modeAjax === 2) {
        baton.modeAjax = 0;
        // keep focus on current scroller when debouncing
        if (type === "scroll") {
            elemScroller.focus();
        }
        await uitableAjax(baton, {});
        return;
    }
    // cleanup
    baton.modeAjax = 0;
    // uitableLoading - hide
    uiFadeOut(elemLoading);
}

function uitableCreate(baton) {
// this function will create a dom-datatable-view of sqlite queries and tables
    let contentElem;
    // All uitables are wrapped in a div
    // Generate the node required for the processing node
    // The HTML structure that we want to generate in this function is:
    //  div - scroller
    //    div - scroll head
    //      div - scroll head inner
    //        table - scroll head table
    //          thead - thead
    //    div - scroll body
    //      table - table (master table)
    //        thead - thead clone for sizing
    //        tbody - tbody
    contentElem = domDivCreate(
        (`
<div class="contentElem" data-init="0" id="${baton.hashtag}">
<div class="contentElemTitle title">${stringHtmlSafe(baton.title)}</div>
<div class="uitableLoading">loading</div>
<div
    class="uichart"
    style="
    display: none;
    height: ${UI_CHART_HEIGHT}px;
    "
>
    <div
        class="uichartNav"
        style="
        height: ${UI_CHART_HEIGHT}px;
        width: ${UI_CHART_LEGEND_WIDTH}px;
        "
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
            style="height: ${UI_CHART_HEIGHT - 64}px;"
        ></div>
    </div>
    <div style="position: relative; margin-left: 16px; width: 16px;">
        <div class="uichartAxis1Label"></div>
    </div>
    <div
        style="display: flex; flex: 1; flex-direction: column; padding: 5px 0"
    >
        <div class="uichartTitle"></div>
        <div
            class="uichartCanvas"
            data-action="uichartZoom"
            style="flex: 1;"
        ></div>
        <div class="uichartAxis0Label"></div>
    </div>
</div>
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
        `)
        + jsonHtmlSafe(baton.colList).map(function (col, ii) {
            return (
                ii === 0
                ? `<th title="${col}" data-sort="asc">${col}</th>`
                : `<th title="${col}">${col}</th>`
            );
        }).join("")
        + (`
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
    // init event-handling - crud
    contentElem.querySelector(
        "tbody"
    ).oncontextmenu = onContextmenu;
    // init event-handling - sorting
    contentElem.querySelector(
        "thead tr"
    ).onclick = uitableSort.bind(undefined, baton);
    // init event-handling - scrolling
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
    // Reset scroll to top in redraw.
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
window.addEventListener("load", init);

export {
    DB_MAIN,
    UI_EDITOR,
    assertOrThrow,
    dbCloseAsync,
    dbExecAsync,
    dbFileAttachAsync,
    dbFileExportAsync,
    dbFileImportAsync,
    dbOpenAsync,
    debugInline,
    domDivCreate,
    jsonHtmlSafe,
    noop,
    onDbExec,
    sqlmathWebworkerInit,
    stringHtmlSafe,
    uiFadeIn,
    uiFadeOut,
    uiRenderDb
};
