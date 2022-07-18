/*jslint browser*/
import {
    DB_MAIN,
    UI_EDITOR,
    dbFileImportAsync,
    onDbExec
} from "./sqlmath_browser.mjs";

await (async function init() {
    let val;
    val = await fetch(".tradebot_public.sqlite");
    val = await val.arrayBuffer();
    await dbFileImportAsync({
        db: DB_MAIN,
        dbData: val
    });
    UI_EDITOR.setValue(
        String(`
DROP TABLE IF EXISTS chart.tradebot_1_holding;
CREATE TABLE chart.tradebot_1_holding (
    datatype TEXT NOT NULL,
    series_index REAL,
    xx REAL,
    yy REAL,
    series_label REAL,
    xx_label TEXT,
    options TEXT
);
INSERT INTO chart.tradebot_1_holding (datatype, options)
    SELECT
        'options' AS datatype,
        '{
            "title": "tradebot holdings",
            "xAxesType": "time",
            "xAxisLabel": "asset",
            "yAxisLabel": "percent holding"
        }' AS options;
INSERT INTO chart.tradebot_1_holding (datatype, xx, xx_label)
    SELECT
        'xx_label' AS datatype,
        rownum AS xx,
        sym AS xx_label
    FROM (
        SELECT
            ROW_NUMBER() OVER (ORDER BY perc_holding DESC) AS rownum,
            sym
        FROM tradebot_position
        WHERE
            perc_holding > 0
        ORDER
            BY perc_holding DESC
    );
INSERT INTO chart.tradebot_1_holding (datatype, series_index, series_label)
    SELECT
        'series_label' AS datatype,
        rownum AS series_index,
        sym AS series_label
    FROM (
        SELECT
            ROW_NUMBER() OVER (ORDER BY sym) AS rownum,
            sym
        FROM (SELECT DISTINCT sym FROM tradebot_position ORDER BY sym)
        WHERE
            sym IS NOT NULL
    );
INSERT INTO chart.tradebot_1_holding (datatype, series_index, xx, yy)
    SELECT
        'yy_value' AS datatype,
        series_index,
        xx,
        perc_holding AS yy
    FROM (
        SELECT
            *
        FROM (
            SELECT
                series_index,
                series_label
            FROM chart.tradebot_1_holding
            WHERE datatype = 'series_label'
        )
        JOIN (
            SELECT
                xx,
                xx_label
            FROM chart.tradebot_1_holding
            WHERE
                datatype = 'xx_label'
        )
    )
    LEFT JOIN tradebot_position
    ON
        sym = series_label
        AND sym = xx_label;
        `).trim()
        + [
            "7 day",
            "1 month",
            "3 month",
            "6 month",
            "1 year",
            "2 year"
        ].map(function (dateInterval, ii) {
            let tableName = (
                `tradebot_historical_2_`
                + `${ii + 1}_${dateInterval.replace(" ", "_")}`
            );
            return String(`
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
                "tradebot historical ${dateInterval} performance vs market",
            "xAxesType": "time",
            "xAxisLabel": "date",
            "yAxisLabel": "percent change"
        }' AS options;
INSERT INTO chart.${tableName} (datatype, xx, xx_label)
    SELECT
        'xx_label' AS datatype,
        rownum AS xx,
        ydate AS xx_label
    FROM (
        SELECT
            ROW_NUMBER() OVER (ORDER BY ydate) AS rownum,
            ydate
        FROM (SELECT DISTINCT ydate FROM tradebot_historical ORDER BY ydate)
        WHERE
            ydate >= DATE('NOW', '-${dateInterval}')
        ORDER BY
            ydate
    );
INSERT INTO chart.${tableName} (datatype, series_index, series_label)
    SELECT
        'series_label' AS datatype,
        rownum AS series_index,
        sym AS series_label
    FROM (
        SELECT
            ROW_NUMBER() OVER (ORDER BY sym) AS rownum,
            sym
        FROM (SELECT DISTINCT sym FROM tradebot_historical ORDER BY sym)
        WHERE
            sym IS NOT NULL
    );
INSERT INTO chart.${tableName} (datatype, series_index, xx, yy)
    SELECT
        'yy_value' AS datatype,
        series_index,
        xx,
        val AS yy
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
            `).trim();
        }).join("\n\n")
    );
    await onDbExec({});
}());
