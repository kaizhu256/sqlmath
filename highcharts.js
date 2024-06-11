/*jslint
    bitwise
    browser
    devel
    for
    long
    nomen
    unordered
*/
/*global
    correctFloat
    arrayMax
    arrayMin
    merge
*/
// init debugInline
let debugInline = (function () {
    let __consoleError = function () {
        return;
    };
    function debug(...argv) {

// This function will print <argv> to stderr and then return <argv>[0].

        __consoleError("\n\ndebugInline");
        __consoleError(...argv);
        __consoleError("\n");
        return argv[0];
    }
    debug(); // Coverage-hack.
    __consoleError = console.error; //jslint-ignore-line
    return debug;
}());
let UI_ANIMATE_DATENOW;
let UI_ANIMATE_DURATION = 250;
let UI_ANIMATE_DURATION_INV = 1 / UI_ANIMATE_DURATION;
let UI_ANIMATE_LIST = [];
let UI_ANIMATE_TIMER_INTERVAL = setInterval(function () {
    UI_ANIMATE_DATENOW = Date.now();
    UI_ANIMATE_LIST = UI_ANIMATE_LIST.filter(function (
        uiAnimateStep
    ) {
        return !uiAnimateStep();
    });
}, 16);
let UI_CHART_COLOR_LIST = [
    "#aa4643",
    "#4572a7",
    "#89a54e",
    "#80699b",
    "#3d96ae",
    "#db843d",
    "#92a8cd",
    "#a47d7c",
    "#b5ca92",
    //
    "#6d9ecf",
    "#3b3b3f",
    "#7ecf6d",
    "#d88f51",
    "#7074cc",
    "#d35170",
    "#c8b94a",
    "#267e7d",
    "#d65050",
    "#7fcbc5"
];
let UI_CHART_CROP_THRESHOLD = 256;
let UI_CHART_MARGIN_BOTTOM = 32;
let UI_CHART_MARGIN_LEFT = 16;
let UI_CHART_MARGIN_RIGHT = 32;
let UI_CHART_MARGIN_TOP = 16;
let UI_CHART_SERIES_LINE_WIDTH = 2;
let UI_CHART_SERIES_POINT_RADIUS = 4;
let UI_CHART_SYMBOL_LIST = [
    "circle", "diamond", "square", "triangle", "triangle-down"
];
let UI_CHART_TOOLTIP_DISTANCE = 12;
/*jslint-disable*/
(function () {
    "use strict";
/*jslint-enable*/
let plotTop = UI_CHART_MARGIN_TOP;
/**
 * Non-recursive method to find the lowest member of an array. Math.min raises a maximum
 * call stack size exceeded error in Chrome when trying to apply more than 150.000 points. This
 * method is slightly slower, but safe.
 */
function arrayMin(data) {
    let ii = data.length;
    let min = data[0];
    while (ii > 0) {
        ii -= 1;
        if (data[ii] < min) {
            min = data[ii];
        }
    }
    return min;
}
/**
 * Non-recursive method to find the lowest member of an array. Math.min raises a maximum
 * call stack size exceeded error in Chrome when trying to apply more than 150.000 points. This
 * method is slightly slower, but safe.
 */
function arrayMax(data) {
    let ii = data.length;
    let max = data[0];
    while (ii > 0) {
        ii -= 1;
        if (data[ii] > max) {
            max = data[ii];
        }
    }
    return max;
}

function assertOrThrow(condition, message) {

// This function will throw <message> if <condition> is falsy.

    if (!condition) {
        throw (
            (!message || typeof message === "string")
            ? new Error(String(message).slice(0, 2048))
            : message
        );
    }
}

function noop(val) {

// This function will do nothing except return <val>.

    return val;
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
            elem.setAttribute(key, val);
            // cache attribute
            switch (key) {
            case "height":
            case "width":
            case "x":
            case "y":
                elem["fx_" + key] = val;
                break;
            }
        }
    });
    return elem;
}

function uiAnimate(elem, attrDict, mode) {
    let datebeg = UI_ANIMATE_DATENOW;
    let fxstateDict = {};
    let {
        childNodes,
        fx_rotate,
        fx_symbolType,
        nodeName
    } = elem;
    let {
        translateY
    } = attrDict;
    // init fxstateDict
    Object.entries(attrDict).forEach(function ([
        fxattr, fxend
    ]) {
        let dpathList;
        let fxbeg;
        let fxattr2 = "fx_" + fxattr;
        let fxstate = {
            fxend
        };
        switch (fxattr) {
        case "d":
            fxbeg = noop(elem.getAttribute(fxattr) ?? "").split(" ");
            dpathList = fxend.split(" ");
            if (!(fxbeg.length > 0 && fxbeg.length === dpathList.length)) {
                elem[fxattr2] = fxend;
                elem.setAttribute(fxattr, fxend);
                return;
            }
            Object.assign(fxstate, {
                fxbeg,
                dpathList
            });
            fxstateDict[fxattr] = fxstate;
            break;
        case "height":
        case "width":
        case "translateX":
        case "translateY":
        case "x":
        case "y":
            fxstate.fxbeg = Number(
                elem[fxattr2] ?? elem.getAttribute(fxattr) ?? 0
            );
            if (fx_symbolType) {
                fxstate.fxbeg = fxstate.fxbeg || fxend;
            }
            fxstateDict[fxattr] = fxstate;
            break;
        case "stroke-width":
        case "visibility":
            elem.setAttribute(fxattr, fxend);
            return;
        default:
            throw new Error(`uiAnimate - invalid attribute - ${fxattr}`);
        }
    });
    function uiAnimateStep() {
        let fxprg = 1;
        let isDone = datebeg + UI_ANIMATE_DURATION <= UI_ANIMATE_DATENOW;
        // animate - linear fxnow, fxprg
        if (!isDone) {
            fxprg = (
                UI_ANIMATE_DURATION_INV
                * (UI_ANIMATE_DATENOW - datebeg)
            );
            if (mode === "easeout") {
                fxprg = Math.sqrt(fxprg);
            }
        }
        Object.entries(fxstateDict).forEach(function ([
            fxattr, fxstate
        ]) {
            let fxattr2 = "fx_" + fxattr;
            let {
                dpathList,
                fxbeg,
                fxend
            } = fxstate;
            let fxnow = fxend;
            switch (fxattr) {
            // Perform the next step of the animation on "d"
            case "d":
                if (!dpathList) {
                    return;
                }
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
                // cache attribute
                elem[fxattr2] = fxnow;
                elem.setAttribute("d", fxnow);
                return;
            case "height":
            case "translateX":
            case "translateY":
            case "width":
            case "x":
            case "y":
                if (!isDone) {
                    fxnow = Math.round(fxbeg + fxprg * (fxend - fxbeg));
                }
                // cache attribute
                elem[fxattr2] = fxnow;
                if (fx_symbolType || translateY) {
                    return;
                }
                // update child tspans x values
                if (fxattr === "x" && nodeName === "text") {
                    childNodes.forEach(function (child) {
                        // if the x values are equal, the tspan represents a
                        // linebreak
                        child.setAttribute("x", fxnow);
                    });
                    if (fx_rotate) {
                        elem.setAttribute(
                            "transform",
                            `rotate(-15 ${fxnow} ${elem.fx_y || 0})`
                        );
                    }
                }
                elem.setAttribute(fxattr, fxnow);
                return;
            }
        });
        if (fx_symbolType) {
            elem.setAttribute("d", svgSymbolpathCreate(
                fx_symbolType,
                elem.fx_x,
                elem.fx_y,
                elem.fx_width,
                elem.fx_height
            ));
        }
        if (translateY) {
            elem.setAttribute(
                "transform",
                `translate(${elem.fx_translateX},${elem.fx_translateY})`
            );
        }
        return isDone;
    }
    // animate - stop existing animation for given elem
    uiAnimateStep.elem2 = elem;
    UI_ANIMATE_LIST = UI_ANIMATE_LIST.filter(function ({
        elem2
    }) {
        return elem2 !== elem;
    });
    // animate - uiAnimateStep()
    uiAnimateStep();
    // animate - setInterval()
    UI_ANIMATE_LIST.push(uiAnimateStep);
}

function numberFormat({
    convert,
    num,
    prefix,
    suffix
}) {
// this function will format <num>
    switch (convert) {
    case "epochToTimeLocal":
        num = new Date(1000 * num).toLocaleTimeString();
        break;
    }
    if (prefix) {
        num = prefix + num;
    }
    if (suffix) {
        num += suffix;
    }
    return num;
}

// uichartCreate0 - start
function uichartCreate0(uichart) {
// this function will create and draw <uichart>
    let axisList = [
        {}, {}
    ];
    let canvasHeight = uichart.elemCanvas.clientHeight;
    let canvasWidth = uichart.elemCanvas.clientWidth;
    let counterColor = 0;
    let counterSymbol = 0;
    let {
        elemCanvas,
        isBarchart,
        seriesList,
        xdataMax,
        xdataMin,
        xlabelList
    } = uichart;
    let [
        xaxis, yaxis
    ] = axisList;
    let elemChart = elemCanvas.firstElementChild;
    let [
        elemClip,
        elemGridlineList,
        elemAxistickList,
        elemSeriesList,
        elemCrosshairList,
        elemTooltip,
        elemMousetrackerList
    ] = elemChart.children;
    let [
        elemTooltipBorder,
        elemTooltipText
    ] = elemTooltip.children;
    let seriesHovered;
    function tooltipRedraw(point) {
    // this function will redraw <elemTooltip> around given <point>
        let {
            plotHeight,
            plotLeft,
            plotWidth
        } = uichart;
        let {
            pointX,
            pointY
        } = point;
        let series = seriesHovered;
        let {
            seriesColor
        } = series;
        let tooltipAlignRight;
        let tooltipBbox;
        let tooltipHeight;
        let tooltipWidth;
        let tooltipX;
        let tooltipY;
        if (uichart.redrawTimer || pointY === undefined) {
            return;
        }
        // update elemTooltipText
        svgAttrSet(elemTooltip, {
            visibility: "visible"
        });
        elemTooltipText.innerHTML = (`
<tspan style="font-size: 10px" x="5">${point.xlabel}</tspan>
<tspan style="fill:#000" dy="17" x="5">${series.seriesName} : </tspan>
<tspan style="font-weight:bold">${Number(point.yy).toLocaleString()}</tspan>
        `);
        // update elemTooltipBorder after text-update
        tooltipBbox = elemTooltipText.getBBox();
        tooltipWidth = tooltipBbox.width + 10;
        tooltipHeight = tooltipBbox.height + 10;
        svgAttrSet(elemTooltipBorder, {
            height: tooltipHeight,
            stroke: seriesColor,
            width: tooltipWidth
        });
        // calculate tooltipX, tooltipY
        // Place the tooltip in a chart without spilling over,
        // and also not covering the point it self.
        tooltipX = pointX + plotLeft - tooltipWidth - UI_CHART_TOOLTIP_DISTANCE;
        tooltipY = pointY + plotTop - 0.5 * tooltipHeight;
        // It is too far to the left, adjust it
        if (tooltipX < plotLeft) {
            tooltipX = pointX + plotLeft + UI_CHART_TOOLTIP_DISTANCE;
        }
        // Test to see if the tooltip is too far to the right, if it is, move
        // it back to be inside and then up to not cover the point.
        if (tooltipX > plotLeft + plotWidth - tooltipWidth) {
            tooltipX = plotLeft + plotWidth - tooltipWidth;
            tooltipY = (
                pointY
                + plotTop
                - tooltipHeight
                - UI_CHART_TOOLTIP_DISTANCE
            );
            tooltipAlignRight = true;
        }
        // If its now above the plot area, align it to the top of the plot area
        tooltipY = Math.max(tooltipY, plotTop + 5);
        // If the tooltip is still covering the point, move it below instead
        if (
            tooltipAlignRight
            && (tooltipY <= pointY && pointY <= tooltipY + tooltipHeight)
        ) {
            tooltipY = pointY + plotTop + UI_CHART_TOOLTIP_DISTANCE;
        }
        // Now if the tooltip is below the chart, move it up.
        // It's better to cover the point than to disappear outside the chart.
        tooltipY = Math.min(tooltipY, (
            plotTop
            + plotHeight
            - tooltipHeight
            - UI_CHART_TOOLTIP_DISTANCE
        ));
        uiAnimate(elemTooltip, {
            translateX: tooltipX,
            translateY: tooltipY
        }, "easeout");
        // redraw crosshair
        axisList.forEach(function (axis, ii) {
            let {
                isXaxis
            } = axis;
            let d = axis.minPixelPadding + axis.transWidth * ((
                isXaxis
                ? point.xx
                : point.yy
            ) - axis.axisMin);
            if (isXaxis) {
                d = Math.round(d) - 0.5;
                d = `M ${d} 0 L ${d} ${canvasHeight}`;
            } else {
                d = Math.round(plotHeight - d) + 0.5;
                d = `M 0 ${d} L ${canvasWidth} ${d}`;
            }
            uiAnimate(elemCrosshairList.children[ii], {
                d,
                visibility: "visible"
            }, "easeout");
        });
    }
    function uichartMouseleave() {
    // this function will handle event when mouse leaves chart-area
        uichartSeriesUnhover();
        svgAttrSet(elemTooltip, {
            visibility: "hidden"
        });
        svgAttrSet(elemCrosshairList.children[0], {
            visibility: "hidden"
        });
        svgAttrSet(elemCrosshairList.children[1], {
            visibility: "hidden"
        });
    }
    function uichartMousemove(evt) {
    // this function will handle event when mouse moves around chart-area
        let mouseX;
        let mouseY;
        let {
            plotHeight,
            plotLeft,
            plotWidth,
            pointHovered
        } = uichart;
        let point;
        let rect;
        // get mouse position mouseX, mouseY
        rect = elemChart.getBoundingClientRect();
        mouseX = Math.round(evt.pageX - rect.left - window.scrollX - plotLeft);
        mouseY = Math.round(
            evt.pageY
            - rect.top
            - window.scrollY
            - plotTop
        );
        if (!(
            0 <= mouseX && mouseX <= plotWidth
            && 0 <= mouseY && mouseY <= plotHeight
        )) {
            return;
        }
        // With line type charts with a single mousetracker,
        // get the point closest to the mouse
        point = (
            seriesHovered
            && seriesHovered.tooltippointList
            && seriesHovered.tooltippointList[mouseX]
        );
        // a new point is hovered, refresh the tooltip
        if (!point || point === pointHovered) {
            return;
        }
        // set normal state to previous series
        if (pointHovered) {
            uichartSeriespointRedraw(
                pointHovered,
                UI_CHART_SERIES_POINT_RADIUS
            );
            delete uichart.pointHovered;
        }
        // update the tooltip
        tooltipRedraw(point);
        // hover this
        uichartSeriespointRedraw(point, UI_CHART_SERIES_POINT_RADIUS + 4);
        uichart.pointHovered = point;
    }
    function uichartSeriesHover(evt) {
    // this function will handle event when mouse hovers over series
        let series = seriesList[evt.currentTarget.__seriesIi];
        if (!series || series === seriesHovered) {
            return;
        }
        // set normal state to previous series
        uichartSeriesUnhover();
        if (!isBarchart) {
            svgAttrSet(series.graph.element, {
                "stroke-width": UI_CHART_SERIES_LINE_WIDTH + 2
            });
        }
        seriesHovered = series;
        uichart.seriesHovered = seriesHovered;
    }
    function uichartSeriesUnhover() {
    // this function will handle event when mouse un-hovers over series
        let pointHovered = uichart.pointHovered;
        // trigger mouse out on the point, which must be in this series
        if (pointHovered) {
            uichartSeriespointRedraw(
                pointHovered,
                UI_CHART_SERIES_POINT_RADIUS
            );
            delete uichart.pointHovered;
        }
        // set normal state
        if (!isBarchart && seriesHovered) {
            svgAttrSet(seriesHovered.graph.element, {
                "stroke-width": UI_CHART_SERIES_LINE_WIDTH
            });
            seriesHovered = undefined;
            uichart.seriesHovered = seriesHovered;
        }
    }
    function uichartSeriespointRedraw(point, radius) {
    // this function will redraw series-point
        if (isBarchart) {
            return;
        }
        let {
            elemPoint
        } = point;
        svgAttrSet(elemPoint, {
            d: svgSymbolpathCreate(
                elemPoint.fx_symbolType,
                point.pointX - radius,
                point.pointY - radius,
                2 * radius,
                2 * radius
            )
        });
    }
    // Expose methods and variables
    Object.assign(uichart, {
        axisList,
        canvasHeight,
        canvasWidth,
        elemAxistickList,
        elemChart,
        elemClip,
        elemCrosshairList,
        elemGridlineList,
        elemMousetrackerList,
        elemSeriesList,
        elemTooltip,
        elemTooltipBorder,
        elemTooltipText,
        isBarchart,
        tooltipX: 0,
        tooltipY: 0,
        xaxis,
        yaxis
    });
    // Resize the box and re-align all aligned elements
    svgAttrSet(elemChart, {
        height: canvasHeight,
        width: canvasWidth
    });
    //
    // The mousetracker object
    // Reference to the chart
    // init event-handling
    elemChart.onmouseleave = uichartMouseleave;
    // When the mouse enters the container, run mouseMove
    elemChart.onmousemove = uichartMousemove;
    uichart.mouseover = uichartSeriesHover;
    //
    // init axisList
    axisList.forEach(function (axis) {
        Object.assign(axis, {
            dataMax: xdataMax,
            dataMin: xdataMin,
            isXaxis: 1,
            tickDict: {},
            tickPositionList: [],
            xOrY: "x"
        });
    });
    // init yaxis
    Object.assign(yaxis, {
        isXaxis: 0,
        xOrY: "y"
    });
    //
    // init seriesList
    seriesList.forEach(function (series, ii) {
        let {
            xdata,
            ydata
        } = series;
        xdata = new Float64Array(xdata);
        ydata = new Float64Array(ydata.map(function (yy) {
            return yy ?? NaN;
        }));
        // set some variables
        Object.assign(series, {
            isVisible: series.isVisible !== false,
            seriespointListCropped: [],
            state: "",
            xdata,
            ydata
        });
        // Get the series' color
        // series.getColor();
        if (typeof series.seriesGroupIndex === "number") {
            series.seriesColor = UI_CHART_COLOR_LIST[
                (series.seriesGroupIndex - 1) % UI_CHART_COLOR_LIST.length
            ];
        } else {
            series.seriesColor = UI_CHART_COLOR_LIST[counterColor];
            counterColor = (counterColor + 1) % UI_CHART_COLOR_LIST.length;
        }
        // Get the series' symbolType
        // series.getSymbol();
        if (!isBarchart) {
            series.symbolType = UI_CHART_SYMBOL_LIST[counterSymbol];
            counterSymbol = (
                (counterSymbol + 1)
                % UI_CHART_SYMBOL_LIST.length
            );
        }
        // pointRange - 1 means xLabel,
        // else auto and least distance between points if not categories
        if (xlabelList.length) {
            series.pointRange = 1;
        } else {
            series.pointRange = series.seriesRangeMin;
        }
        series.seriesName = series.seriesName || `series ${(ii + 1)}`;
    });
    // render seriesList in reverse, so first series has highest z-index
    Array.from(seriesList).reverse().forEach(function (series) {
        series.elemSeries = svgAttrSet("g");
        elemSeriesList.appendChild(series.elemSeries);
        // init elemSeriestrackerList
        //
        series.elemSeriestrackerList = svgAttrSet("g");
        elemMousetrackerList.appendChild(series.elemSeriestrackerList);
    });
    // first-pass - init axis
    uichartRedraw(uichart, "init");
    // second-pass - draw chart
    uichartRedraw(uichart, true);
    // startup-animation
    svgAttrSet(elemClip.firstElementChild, {
        width: 0
    });
    uiAnimate(elemClip.firstElementChild, {
        width: uichart.plotWidth
    });
    return uichart;
}
// uichartCreate0 - end

function uichartSeriesHideOrShow({
    isVisible,
    series
}) {
// this function will hide-or-show <series>
    series.isVisible = isVisible;
    [
        series.elemSeries, series.elemSeriestracker
    ].forEach(function (elem) {
        if (elem) {
            svgAttrSet(elem, {
                visibility: (
                    isVisible
                    ? "visible"
                    : "hidden"
                )
            });
        }
    });
}

function svgSymbolpathCreate(symbolType, x, y, w, h) {
// this function will create svg-dpath for given <symbolType>
    let tmp;
    switch (symbolType) {
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

// uichartRedraw - start
function uichartRedraw(uichart, modeDebounce) {
// this function will redraw <uichart>
    if (!modeDebounce) {
        clearTimeout(uichart.redrawTimer);
        uichart.redrawTimer = setTimeout(
            uichartRedraw,
            0,
            uichart,
            true
        );
        return;
    }
    delete uichart.redrawTimer;
    let {
        axisList,
        canvasHeight,
        canvasWidth,
        elemAxistickList,
        elemClip,
        elemCrosshairList,
        elemGridlineList,
        elemMousetrackerList,
        elemSeriesList,
        isBarchart,
        plotBottom,
        plotHeight,
        plotLeft = 0,
        plotRight,
        plotWidth,
        seriesList,
        xdataMax,
        xdataMin,
        xlabelList,
        xrangeMin,
        xzoomMax,
        xzoomMin
    } = uichart;
    let [
        xaxis, yaxis
    ] = axisList;
    let xaxisMaxOld = xaxis.axisMax;
    let xaxisMinOld = xaxis.axisMin;
    function axisTranslate(axis, val, isCanvasCoordinate) {
    // Translate from axis value to pixel position on the chart, or back
        val = axis.minPixelPadding + axis.transWidth * (val - axis.axisMin);
        return Math.round(
            isCanvasCoordinate
            ? -val + axis.axisLength
            : val
        );
    }
    function tickCreate(axis, pos) {
    // this function will create tick and ticklabel at given <pos> in <axis>
        let elemTick;
        let elemTicklabel;
        let elemTspan;
        let {
            isXaxis
        } = axis;
        let xx = plotLeft + 0.5 * plotWidth;
        let yy = canvasHeight - plotBottom - 0.5 * plotHeight;
        if (isXaxis) {
            yy = canvasHeight + 0.125 * plotHeight;
        } else {
            xx = -0.125 * plotWidth;
        }
        // create elemTick
        elemTick = svgAttrSet("path", {
            fill: "none",
            stroke: "#333",
            "stroke-width": 1,
            x: xx,
            y: yy
        });
        if (isXaxis) {
            elemAxistickList.appendChild(elemTick);
        }
        // create elemGridline
        if (!isXaxis) {
            elemTick.elemGridline = svgAttrSet("path", {
                d: [
                    "M",
                    plotLeft,
                    yy,
                    "L",
                    canvasWidth - plotRight,
                    yy
                ].join(" "),
                fill: "none",
                stroke: "#bbb",
                "stroke-width": 1
            });
            elemGridlineList.appendChild(elemTick.elemGridline);
        }
        // skip ticklabel, if elemTick is not contained in xlabelList
        if (
            isXaxis
            && xlabelList.length
            && !xlabelList.hasOwnProperty(pos - 1)
        ) {
            return elemTick;
        }
        // create ticklabel
        elemTspan = svgAttrSet("tspan");
        elemTspan.textContent = (function () {
            let num;
            let numDigitList;
            num = numberFormat({
                convert: uichart[axis.xOrY + "valueConvert"],
                num: (
                    isXaxis
                    ? xlabelList[pos - 1] ?? pos
                    : pos
                ),
                prefix: uichart[axis.xOrY + "valuePrefix"],
                suffix: uichart[axis.xOrY + "valueSuffix"]
            });
            // number already formatted
            if (typeof num !== "number") {
                return num;
            }
            // small number
            if (Math.abs(num) < 1000) {
                return num.toLocaleString();
            }
            // large number
            num = Math.round(num).toLocaleString();
            numDigitList = num.split(
                /[^+\-0-9]/
            );
            switch (numDigitList.length) {
            // kilo
            case 2:
                return numDigitList[0] + "k";
            // mega
            case 3:
                return numDigitList[0] + "M";
            // giga
            case 4:
                return numDigitList[0] + "G";
            // tera
            case 5:
                return numDigitList[0] + "T";
            // peta
            case 6:
                return numDigitList[0] + "P";
            // exa
            case 7:
                return numDigitList[0] + "E";
            default:
                return num;
            }
        }());
        elemTicklabel = Object.assign(svgAttrSet("text", {
            "text-anchor": (
                isXaxis
                ? "middle"
                : "end"
            ),
            x: xx,
            y: yy
        }), {
            fx_rotate: isXaxis
        });
        elemTick.elemTicklabel = elemTicklabel;
        elemTicklabel.appendChild(elemTspan);
        elemAxistickList.appendChild(elemTicklabel);
        return elemTick;
    }
    function uichartRedrawPlotRectGet(uichart) {
        let {
            tickDict = {},
            tickPositionList = []
        } = yaxis;
        // calculate plotLeft
        plotLeft = 16;
        tickPositionList.forEach(function (pos) {
            if (!tickDict[pos]) {
                tickDict[pos] = tickCreate(yaxis, pos);
            }
            plotLeft = Math.max(
                plotLeft,
                tickDict[pos].elemTicklabel.getBBox().width
            );
        });
        plotLeft = Math.round(plotLeft + UI_CHART_MARGIN_LEFT);
        // calculate plotHeight, plotWidth
        plotHeight = Math.round(
            canvasHeight
            - plotTop
            - UI_CHART_MARGIN_BOTTOM
        );
        plotWidth = Math.round(canvasWidth - plotLeft - UI_CHART_MARGIN_RIGHT);
        // calculate plotBottom, plotRight
        plotBottom = canvasHeight - plotHeight - plotTop;
        plotRight = canvasWidth - plotWidth - plotLeft;
        Object.assign(uichart, {
            plotBottom,
            plotHeight,
            plotLeft,
            plotRight,
            plotWidth
        });
        // update axis
        axisList.forEach(function (axis) {
            let {
                isXaxis
            } = axis;
            axis.axisLength = (
                isXaxis
                ? plotWidth
                : plotHeight
            );
        });
        // update crop-area
        svgAttrSet(elemClip.firstElementChild, {
            height: plotHeight,
            width: plotWidth
        });
        // crop crosshair, mouse-tracker, series
        [
            elemCrosshairList, elemMousetrackerList, elemSeriesList
        ].forEach(function (elem) {
            svgAttrSet(elem, {
                "clip-path": `url(#${elemClip.id})`,
                transform: `translate(${plotLeft},${plotTop})`
            });
        });
    }
    function uichartRedrawSeries(series) {
    // this function will redraw given <series>
        // Generate the data point after the data has been processed by
        // cropping away unused points
        let {
            seriespointListCropped,
            xcropEnd,
            xcropStart,
            xdata,
            ydata
        } = series;
        let ii;
        let nn;
        let point;
        let seriespointList = [];
        ii = xcropStart;
        while (ii < xcropEnd) {
            seriespointListCropped[ii] = seriespointListCropped[ii] || {
                xx: xdata[ii],
                yy: (
                    Number.isNaN(ydata[ii])
                    ? undefined
                    : ydata[ii]
                )
            };
            seriespointList[ii - xcropStart] = seriespointListCropped[ii];
            ii += 1;
        }
        // Hide cropped-away points - this only runs when the number of points
        // is above cropThreshold, or when swithching view from non-grouped
        // data to grouped data (#637)
        ii = 0;
        nn = seriespointListCropped.length;
        while (ii < nn) {
            if (ii === xcropStart) {
                ii += xcropEnd - xcropStart;
            }
            point = seriespointListCropped[ii];
            if (point) {
                delete point.pointX;
            }
            if (point && point.elemPoint) {
                point.elemPoint.remove();
                delete point.elemPoint;
            }
            ii += 1;
        }
        series.seriespointList = seriespointList;
        //
        //
        let seriessegmentList = [];
        let seriessegmentStart = 0;
        // now that we have the cropped data, build the segments
        // Divide the series data into segments divided by null values.
        // split on null points
        seriespointList.forEach(function (point, ii) {
            let {
                xx,
                yy
            } = point;
            // Translate each point
    /**
     * Translate data points from raw data values to chart specific positioning data
     * needed later in drawPoints, drawGraph and drawTracker.
     */
            Object.assign(point, {
                pointX: axisTranslate(xaxis, xx, 0),
                pointY: (
                    yy === undefined
                    ? undefined
                    : 0.1 * Math.round(10 * axisTranslate(yaxis, yy, 1))
                ),
                xlabel: xlabelList[xx - 1] ?? xx
            });
            if (yy === undefined) {
                if (ii > seriessegmentStart) {
                    seriessegmentList.push(
                        seriespointList.slice(seriessegmentStart, ii)
                    );
                }
                seriessegmentStart = ii + 1;
                return;
            }
            // last value
            if (ii === seriespointList.length - 1) {
                seriessegmentList.push(
                    seriespointList.slice(seriessegmentStart, ii + 1)
                );
            }
        });
        // register it
        series.seriessegmentList = seriessegmentList;
        // barchart
        // Translate each point to the plot area coordinate system and find shape positions
        if (!isBarchart) {
            return;
        }
        let columnCount = 0;
        // Get the total number of column type series.
        // This is called on every series. Consider moving this logic to a
        // chart.orderStacks() function and call it on init, addSeries and removeSeries
        if (uichart.seriesGrouping === false) {
            columnCount = 1;
        // } else {
        //     let columnIndex;
        //     seriesList.forEach(function (otherSeries) {
        //         // used in Stock charts navigator series
        //         if (
        //             otherSeries.isVisible
        //             && series.elemSeries === otherSeries.elemSeries
        //         ) {
        //             if (uichart.seriesGrouping !== false) {
        //                 columnIndex = columnCount;
        //                 columnCount += 1;
        //             }
        //             otherSeries.columnIndex = columnIndex;
        //         }
        //     });
        }
        // calculate the width and position of each column based on
        // the number of column series in the plot, the groupPadding
        let xlabelWidth = (
            Math.abs(xaxis.transWidth)
            * (series.pointRange || 1)
        );
        let pointOffsetWidth = (1 - 2 * 0.2) * xlabelWidth / columnCount;
        // exact point width, used in polar charts
        let pointWidth = pointOffsetWidth - 2 * (0.1 * pointOffsetWidth);
        // rounded and postprocessed for border width
        let barW = Math.ceil(Math.max(pointWidth, 5));
        let colIndex = (series.columnIndex) || 0;
        let pointXOffset = (
            0.1 * pointOffsetWidth
            + (
                0.2 * xlabelWidth
                + colIndex * pointOffsetWidth
                - 0.5 * xlabelWidth
            )
        );
        // Get the zero plane either based on zero or on the min or max value.
        // Used in bar and area plots
        let threshold = Math.max(yaxis.axisMin, Math.min(yaxis.axisMax, 0));
        let translatedThreshold = axisTranslate(yaxis, threshold, 1);
        series.translatedThreshold = translatedThreshold;
        let minPointLength = 4;
        // record the new values
        seriespointList.forEach(function (point) {
            let pointY = Math.min(Math.max(-999, point.pointY), yaxis.axisLength + 999); // Don't draw too far outside plot area (#1303)
            let barX = point.pointX + pointXOffset;
            let barY = Math.ceil(Math.min(pointY, translatedThreshold));
            let barH = Math.ceil(Math.max(pointY, translatedThreshold) - barY);
            let shapeArgs;
            if (Math.abs(barH) < minPointLength) {
                barH = minPointLength;
            }
            point.barX = barX;
            point.pointWidth = pointWidth;
            // create shape type and shape args that are reused in drawPoints and drawTracker
            // normalize for crisp edges
            shapeArgs = {
                height: Math.floor(barH - 1),
                "stroke-width": 1,
                width: Math.floor((barW) - 1),
                x: Math.floor(barX) + 0.5,
                y: Math.floor(barY) + 0.5
            };
            point.shapeArgs = shapeArgs;
        });
    }
    function uichartRedrawSeriespointList(series) {
        let {
            elemSeries,
            seriesColor,
            seriespointList,
            symbolType
        } = series;
        /**
         * Draw the columns. For bars, the series.group is rotated, so the same coordinates
         * apply for columns and bars. This method is inherited by scatter series.
         *
         */
        seriespointList.forEach(function (point, ii) {
            let {
                elemPoint,
                pointX,
                pointY,
                shapeArgs
            } = point;
            let radius;
            let isInside;
            // only draw the point if y is defined
            if (pointY === undefined) {
                if (elemPoint) {
                    elemPoint = elemPoint.remove();
                }
                return;
            }
            // draw the columns
            if (isBarchart) {
                if (elemPoint) { // update
                    uiAnimate(elemPoint, shapeArgs);
                    return;
                }
                //  Draw and return a rectangle
                elemPoint = svgAttrSet("rect", Object.assign({}, shapeArgs, {
                    fill: seriesColor,
                    stroke: "#fff",
                    "stroke-width": 0.5
                }));
                //
                point.elemPoint = elemPoint;
                series.elemSeries.appendChild(elemPoint);
                elemPoint.__seriespointIi = ii;
                return;
            }
            // Draw the markers
            isInside = (
                0 <= pointX && pointX <= plotWidth
                && 0 <= pointY && pointY <= plotHeight
            );
            // shortcuts
            radius = 4;
            // update
            if (elemPoint) {
                // Since the marker group isn't clipped, each individual marker
                // must be toggled
                svgAttrSet(elemPoint, {
                    visibility: (
                        isInside
                        ? "inherit"
                        : "hidden"
                    )
                });
                uiAnimate(elemPoint, {
                    height: 2 * radius,
                    width: 2 * radius,
                    x: pointX - radius,
                    y: pointY - radius
                });
            } else if (isInside) {
                // Draw a symbol out of pre-defined shape paths from the
                // namespace "symbol" object.
                elemPoint = svgAttrSet("path", {
                    d: svgSymbolpathCreate(
                        symbolType,
                        pointX - radius,
                        pointY - radius,
                        2 * radius,
                        2 * radius
                    ),
                    fill: seriesColor,
                    "stroke-width": 0
                });
                elemPoint.fx_symbolType = symbolType;
                point.elemPoint = elemPoint;
                elemSeries.appendChild(elemPoint);
            }
        });
    }
    function uichartRedrawSeriestracker(series) {
    /**
     * Draw the seriestracker object that sits above all data labels and markers to
     * track mouse events on the graph or points. For the line type charts
     * the seriestracker uses the same linePath, but with a greater stroke width
     * for better control.
     */
        let ii;
        let {
            elemSeriestracker,
            elemSeriestrackerList,
            seriespointList
        } = series;
        if (isBarchart) {
            seriespointList.forEach(function (point) {
                let shapeArgs;
                if (point.pointY === undefined) {
                    return;
                }
                shapeArgs = Object.assign({}, point.shapeArgs, {
                    height: 1999,
                    y: -999
                });
                if (elemSeriestracker) {
                    svgAttrSet(elemSeriestracker, shapeArgs);
                    return;
                }
                elemSeriestracker = svgAttrSet("rect");
                series.elemSeriestracker = elemSeriestracker;
                svgAttrSet(elemSeriestracker, Object.assign({}, shapeArgs, {
                    fill: "rgba(192,192,192,0.0001)",
                    visibility: (
                        series.isVisible
                        ? "visible"
                        : "hidden"
                    )
                }));
                // init event-handling
                elemSeriestracker.__seriesIi = seriesList.indexOf(series);
                elemSeriestracker.onmouseover = uichart.mouseover;
                elemSeriestrackerList.appendChild(elemSeriestracker);
            });
            return;
        }
        let mousetrackerPath = [].concat(series.linePath);
        let mousetrackerPathLength = mousetrackerPath.length;
        let snap = 10;
        // Extend end points. A better way would be to use round linecaps,
        // but those are not clickable in VML.
        if (mousetrackerPathLength) {
            ii = mousetrackerPathLength + 1;
            while (ii > 0) {
                ii -= 1;
                if (mousetrackerPath[ii] === "M") { // extend left side
                    mousetrackerPath.splice(ii + 1, 0, mousetrackerPath[ii + 1] - snap, mousetrackerPath[ii + 2], "L");
                }
                if ((ii && mousetrackerPath[ii] === "M") || ii === mousetrackerPathLength) { // extend right side
                    mousetrackerPath.splice(ii, 0, "L", mousetrackerPath[ii - 2] + snap, mousetrackerPath[ii - 1]);
                }
            }
        }
        // handle single points
        series.singlePoints.forEach(function (singlePoint) {
            mousetrackerPath.push(
                "M",
                singlePoint.pointX - snap,
                singlePoint.pointY,
                "L",
                singlePoint.pointX + snap,
                singlePoint.pointY
            );
        });
        // draw the seriestracker
        if (elemSeriestracker) {
            svgAttrSet(elemSeriestracker, {
                d: mousetrackerPath.join(" ")
            });
            return;
        }
        // create
        elemSeriestracker = svgAttrSet("path");
        series.elemSeriestracker = elemSeriestracker;
        //
        svgAttrSet(elemSeriestracker, {
            d: mousetrackerPath.join(" "),
            fill: "none",
            stroke: "rgba(192,192,192,0.0001)",
            "stroke-linejoin": "round",
            "stroke-width": UI_CHART_SERIES_LINE_WIDTH + 2 * snap,
            visibility: (
                series.isVisible
                ? "visible"
                : "hidden"
            )
        });
        // init event-handling
        elemSeriestracker.__seriesIi = seriesList.indexOf(series);
        elemSeriestracker.onmouseover = uichart.mouseover;
        elemSeriestrackerList.appendChild(elemSeriestracker);
    }
    // redraw axisList - start
    uichartRedrawPlotRectGet(uichart);
    // calculate axisMax, axisMin from axisList
    axisList.forEach(function (axis) {
        if (uichart.isResizing) {
            return;
        }
        let {
            isXaxis
        } = axis;
        let axisMax;
        let axisMin;
        let tickInterval;
        let tickPositionList;
        // calculate xaxisMax, xaxisMin
        (function () {
            let xaxisWidth;
            let xzoomOffset;
            if (!isXaxis) {
                return;
            }
            axisMax = Math.min(xdataMax, xzoomMax ?? xdataMax);
            axisMin = Math.max(xdataMin, xzoomMin ?? xdataMin);
            xzoomOffset = 0.5 * (xrangeMin - axisMax + axisMin);
            // if xrangeMin is exceeded, adjust
            if (axisMax - axisMin < xrangeMin) {
                axisMax = Math.min(xdataMax, axisMin + xrangeMin);
                axisMin = Math.max(xdataMin, axisMin - xzoomOffset);
                if (axisMax - axisMin < xrangeMin) {
                    axisMin = Math.max(axisMax - xrangeMin);
                }
            }
            xaxisWidth = axisMax - axisMin;
            // pad axisMax, axisMin
            axisMax += 0.02 * xaxisWidth;
            axisMin -= 0.02 * xaxisWidth;
        }());
        // if isBarchart, then make sure yaxis is visible
        if (!isXaxis && isBarchart) {
            axisMax = 0;
            axisMin = 0;
        }
        // calculate xcropEnd, xcropStart, yaxisMax, yaxisMin
        seriesList.forEach(function (series) {
            let {
                xcropEnd,
                xcropStart,
                xdata,
                ydata
            } = series;
            let ii;
            let nn = xdata.length;
            let xaxisMax = xaxis.axisMax;
            let xaxisMin = xaxis.axisMin;
            let yy;
            if (isXaxis) {
                return;
            }
            // calculate xcropEnd, xcropStart
            // if xaxis is unchanged, then skip
            if (xaxisMax === xaxisMaxOld && xaxisMin === xaxisMinOld) {
                noop();
            // xdata inside xcrop-range
            } else if (xaxisMin <= xdata[0] && xdata[nn - 1] <= xaxisMax) {
                xcropEnd = nn;
                xcropStart = 0;
            // xdata outside xcrop-range
            } else if (xdata[nn - 1] < xaxisMin || xaxisMax < xdata[0]) {
                xcropEnd = 0;
                xcropStart = 0;
            // init xcropStart
            } else {
                ii = 0;
                while (ii < nn && xdata[ii] < xaxisMin) {
                    ii += 1;
                }
                xcropStart = Math.max(0, ii - 1);
                // init xcropEnd
                while (ii < nn && xdata[ii] < xaxisMax) {
                    ii += 1;
                }
                xcropEnd = ii;
            }
            // calculate yaxisMax, yaxisMin
            if (series.isVisible) {
                ii = xcropStart;
                while (ii < xcropEnd) {
                    yy = ydata[ii];
                    if (!Number.isNaN(yy)) {
                        axisMax = Math.max(axisMax ?? yy, yy);
                        axisMin = Math.min(axisMin ?? yy, yy);
                    }
                    ii += 1;
                }
            }
            axisMax = axisMax ?? 0;
            axisMin = axisMin ?? 0;
            // save xcropEnd, xcropStart
            if (nn <= UI_CHART_CROP_THRESHOLD) {
                xcropEnd = nn;
                xcropStart = 0;
            }
            series.xcropEnd = xcropEnd;
            series.xcropStart = xcropStart;
        });
        // calculate tickInterval
        (function () {
            let magnitude;
            let multiple;
            tickInterval = (
                isXaxis
                ? (axisMax - axisMin) * 100 / plotWidth
                : (axisMax - axisMin) * 72 / plotHeight
            ) || 1;
            magnitude = Math.pow(
                10,
                Math.floor(Math.log(tickInterval) / Math.LN10)
            );
            tickInterval = tickInterval / magnitude;
            // round to a tenfold of 1, 2, 2.5 or 5
            Array.from([
                1, 2, 2.5, 5, 10
            ]).some(function (__multiple, ii, list) {
                multiple = __multiple;
                return (
                    2 * tickInterval <= multiple + (list[ii + 1] || multiple)
                );
            });
            // multiply back to the correct magnitude
            tickInterval = multiple * magnitude;
        }());
        // calculate tickPositionList
        (function () {
            let pos;
            let posMax;
            let posMin;
            let posPrev;
            function floatCorrect(num) {
            // this function will correct float-error in <num>
                return parseFloat(
                    num.toPrecision(12)
                );
            }
            // Set the tick positions of a linear axis to round values like
            // whole tens or every five.
            posMin = floatCorrect(
                Math.floor(axisMin / tickInterval) * tickInterval
            );
            posMax = floatCorrect(
                Math.ceil(axisMax / tickInterval) * tickInterval
            );
            tickPositionList = [];
            // Populate the intermediate values
            pos = posMin;
            while (pos <= posMax) {
                // Place the tick on the rounded value
                tickPositionList.push(pos);
                // Always add the raw tickInterval, not the corrected one.
                pos = floatCorrect(pos + tickInterval);
                // If the interval is not big enough in the current min - max
                // range to actually increase the loop variable, we need to
                // break out to prevent endless loop.
                if (pos === posPrev) {
                    break;
                }
                // Record the last value
                posPrev = pos;
            }
            // reset min/max or remove extremes based on start/end on tick
            posMin = tickPositionList[0];
            posMax = tickPositionList[tickPositionList.length - 1];
            if (!isXaxis) {
                axisMin = posMin;
            } else if (axisMin - axis.minPointOffset > posMin) {
                tickPositionList.shift();
            }
            if (!isXaxis) {
                axisMax = posMax;
            } else if (axisMax + axis.minPointOffset < posMax) {
                tickPositionList.pop();
            }
            // When there is only one point, or all points have the same value
            // on this axis, then min and max are equal and
            // tickPositionList.length is 1. In this case, add some padding in
            // order to center the point, but leave it with one tick.
            if (tickPositionList.length === 1) {
                // The lowest possible number to avoid extra padding on columns
                axisMin -= 1e-9;
                axisMax += 1e-9;
            }
        }());
        // save axisMax, axisMin
        axis.axisMax = axisMax;
        axis.axisMin = axisMin;
        // save tickInterval, tickPositionList
        axis.tickInterval = tickInterval;
        axis.tickPositionList = tickPositionList;
    });
    uichartRedrawPlotRectGet(uichart); // tickPositionList modified
    // redraw axisList - end
    //
    //!! uichart.setChartSize();
    // handle automatic or user set offset
    // Set the public chart properties. This is done before and after the
    // pre-render to determine margin sizes
    axisList.forEach(function (axis) {
        let {
            axisLength,
            isXaxis
        } = axis;
        //
        //
        // Update translation information
        //!! axis.setAxisTranslation();
        let range = axis.axisMax - axis.axisMin;
        let pointRange = 0;
        let minPointOffset = 0;
        let transWidth = axis.transWidth;
        // adjust translation for padding
        if (isXaxis) {
            seriesList.forEach(function (series) {
                pointRange = Math.max(pointRange, series.pointRange);
                // minPointOffset is the value padding to the left of the axis in order to make
                // room for points with a pointRange, typically columns. When the pointPlacement option
                // is "between" or "on", this padding does not apply.
                minPointOffset = Math.max(
                    minPointOffset,
                    0.5 * series.pointRange
                );
                // Determine the total padding needed to the length of the axis to make room for the
                // pointRange. If the series' pointPlacement is "on", no padding is added.
            });
            // Record minPointOffset and pointRangePadding
            axis.minPointOffset = minPointOffset;
            // pointRange means the width reserved for each point, like in a column chart
            axis.pointRange = pointRange;
        }
        // secondary values
        transWidth = axisLength / ((range + pointRange) || 1);
        axis.transWidth = transWidth;
        axis.minPixelPadding = transWidth * minPointOffset;
        //
        //
    });
    // redraw elemGridline, elemTick, elemTicklabel in tickPositionList
    axisList.forEach(function (axis) {
        let {
            axisMin,
            isXaxis,
            minPixelPadding,
            tickDict,
            tickPositionList,
            transWidth
        } = axis;
        tickPositionList.forEach(function (tickX) {
            let elemTick;
            if (!tickDict.hasOwnProperty(tickX)) {
                tickDict[tickX] = tickCreate(axis, tickX);
            }
            elemTick = tickDict[tickX];
            let {
                elemGridline,
                elemTicklabel
            } = elemTick;
            let xx;
            let yy;
            // init xx, yy
            xx = plotLeft;
            yy = canvasHeight - plotBottom;
            if (isXaxis) {
                xx += minPixelPadding + (tickX - axisMin) * transWidth;
            } else {
                yy -= minPixelPadding + (tickX - axisMin) * transWidth;
            }
            // crisp
            xx = Math.round(xx) - 0.5;
            yy = Math.round(yy) + 0.5;
            // redraw elemTick
            if (isXaxis) {
                uiAnimate(elemTick, {
                    d: `M ${xx} ${yy} L ${xx} ${yy + 5}`
                });
            }
            // redraw elemTicklabel
            if (elemTicklabel) {
                uiAnimate(elemTicklabel, (
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
            // redraw elemGridline
            if (elemGridline) {
                uiAnimate(elemGridline, {
                    d: `M ${plotLeft} ${yy} L ${canvasWidth - plotRight} ${yy}`
                });
            }
            elemTick.isVisible = true;
        });
        // cleanup undrawn elemGridline, elemTick, elemTicklabel
        Object.entries(tickDict).forEach(function ([
            tickX, elemTick
        ]) {
            elemTick.isVisible = !elemTick.isVisible;
            if (!elemTick.isVisible) {
                return;
            }
            elemTick.remove();
            if (elemTick.elemGridline) {
                elemTick.elemGridline.remove();
            }
            if (elemTick.elemTicklabel) {
                elemTick.elemTicklabel.remove();
            }
            delete tickDict[tickX];
        });
    });
    // if first-render, redraw axis
    if (modeDebounce === "init") {
        return;
    }
    //
    //
    // redraw affected series
    seriesList.forEach(function (series) {
        // cache it here as it is set to false in render, but used after
        // Redraw the series after an update in the axes.
        // reposition on resize
        uichartRedrawSeries(series);
        //
        //
        //!! series.setTooltipPoints();
        // Memoize tooltip texts and positions
        let pointList = [];
        let low;
        let high;
        // a lookup array for each pixel in the x dimension
        let tooltippointList = [];
        // concat segments to overcome null values
        pointList = series.seriessegmentList.flat();
        // Assign each pixel position to the nearest point
        pointList.forEach(function (point, ii) {
            // Set this range's low to the last range's high plus one
            low = (
                pointList[ii - 1]
                ? high + 1
                : 0
            );
            // Now find the new high
            high = (
                pointList[ii + 1]
                ? Math.max(0, Math.floor(0.5 * (
                    point.pointX
                    + (
                        pointList[ii + 1]
                        ? pointList[ii + 1].pointX
                        : plotWidth
                    )
                )))
                : plotWidth
            );
            while (0 <= low && low <= high) {
                tooltippointList[low] = point;
                low += 1;
            }
        });
        series.tooltippointList = tooltippointList;
        //
        //
        // Render the graph and markers
        // the group
        //
        //
        //
        // draw the graph if any
        if (!isBarchart) {
            let {
                elemSeries,
                graph,
                seriesColor,
                seriessegmentList
            } = series;
            // Get the graph path
            let linePath = [];
            let singlePoints = []; // used in drawTracker
            // Divide into segments and build graph and area paths
            seriessegmentList.forEach(function (segment) {
                // add a single point for tracking
                if (segment.length <= 1) {
                    singlePoints.push(segment[0]);
                    return;
                }
                // add the segment to the graph, or a single point for tracking
                // Return the graph path of a segment
                // build the segment line
                segment.forEach(function (point, ii) {
                    if (point.pointY !== undefined) {
                        linePath.push(
                            // moveTo or lineTo
                            (
                                ii
                                ? "L"
                                : "M"
                            ),
                            // normal line to next point
                            point.pointX,
                            point.pointY
                        );
                    }
                });
            });
            // Record it for use in drawGraph and drawTracker,
            // and return linePath
            series.singlePoints = singlePoints;
            series.linePath = linePath;
            // draw the graph
            if (!graph) {
                graph = {
                    element: svgAttrSet("path", {
                        d: linePath.join(" "),
                        fill: "none"
                    })
                };
                svgAttrSet(graph.element, {
                    stroke: seriesColor,
                    "stroke-width": UI_CHART_SERIES_LINE_WIDTH
                });
                series.graph = graph;
                elemSeries.appendChild(graph.element);
            } else {
                uiAnimate(graph.element, {
                    d: linePath.join(" ")
                });
            }
        }
        // draw the points
        uichartRedrawSeriespointList(series);
        // draw the mouse tracking area
        uichartRedrawSeriestracker(series);
    });
}
// uichartRedraw - end

function uichartResize(uichart) {
// this function will resize <uichart>
    let canvasHeight;
    let canvasWidth;
    let {
        elemChart,
        elemCanvas
    } = uichart;
    // temporarily remove elemChart to recalculate canvasHeight, canvasWidth
    elemChart.remove();
    canvasHeight = elemCanvas.clientHeight;
    canvasWidth = elemCanvas.clientWidth;
    elemCanvas.appendChild(elemChart);
    Object.assign(uichart, {
        canvasHeight,
        canvasWidth
    });
    // increment / decrement isResizing
    uichart.isResizing += 1;
    setTimeout(function () {
        uichart.isResizing -= 1;
    }, UI_ANIMATE_DURATION);
    svgAttrSet(elemChart, {
        height: canvasHeight,
        width: canvasWidth
    });
    // redraw from uichartResize
    uichartRedraw(uichart);
}

Object.assign(window, {
    uichartCreate0,
    uichartRedraw,
    uichartResize,
    uichartSeriesHideOrShow
});
/*jslint-disable*/
}());
/*jslint-enable*/
