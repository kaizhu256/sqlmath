/*jslint bitwise, browser, nomen, unordered*/
/*global
    Axis
    LineSeries
    SVGElement
    attr
    correctFloat
    destroyObjectProperties
    pick
*/
"use strict"; //jslint-ignore-line
let DBCHART_DICT = new Map();
//!! debug
window.DBCHART_DICT = DBCHART_DICT;
let DEBOUNCE_DICT = Object.create(null);
let DOM_ID_COUNTER = 0;
let Highcharts = {};
let PREFIX = "uichart-";
let UI_CHART_HEIGHT = 384;
let UI_CHART_LEGEND_WIDTH = 196;
let UI_ANIMATE_TIMERID;
let UI_ANIMATE_TIMERLIST = [];
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
// The Highcharts namespace
window.Highcharts = Highcharts;
function attr(elem, prop, value) {
// Set or get an attribute or an object of attributes.
    let setAttribute = "setAttribute";
    let ret;
    // if the prop is a string
    if (typeof prop === "string") {
        // set the value
        if (isDefined(value)) {
            elem[setAttribute](prop, value);
        // get the value
        // elem not defined when printing pie demo...
        } else if (elem && elem.getAttribute) {
            ret = elem.getAttribute(prop);
        }
    // else if prop is defined, it is a hash of key/value pairs
    } else if (isDefined(prop) && typeof prop === "object") {
        Object.entries(prop).forEach(function ([
            key, val
        ]) {
            elem[setAttribute](key, val);
        });
    }
    return ret;
}
function pick(...argList) {
// Return the first value that is defined. Like MooTools' $.pick.
    let arg;
    let ii = 0;
    while (ii < argList.length) {
        arg = argList[ii];
        if (arg !== null && arg !== undefined) {
            return arg;
        }
        ii += 1;
    }
}
function destroyObjectProperties(obj) {
// Utility method that destroys any SVGElement or VMLElement that are
// properties on the given object. It loops all properties and invokes destroy
// if there is a destroy method. The property is then delete'ed.
    Object.entries(obj).forEach(function ([
        key, wrapper
    ]) {
        let element = wrapper.element;
        if (element) {
            uiAnimateStop(wrapper);
            // remove element
            if (element.parentNode) {
                element.parentNode.removeChild(element);
            }
        }
        delete obj[key];
    });
}
function correctFloat(num) {
// Fix JS round off float errors
// @param {Number} num
    return parseFloat(num.toPrecision(12));
}
/*jslint-disable*/
/* ****************************************************************************
// Handle the options                                                         *
 *****************************************************************************/
// Series defaults
/**
// A wrapper object for SVG elements
 */
function SVGElement() {}
SVGElement.prototype = {
    /**
     * @param {Object} params
     * @param {Number} options The same options as in jQuery animation
     * @param {Function} complete Function to perform at the end of animation
     */
    animate: function (params) {
        // Animate a given attribute
        // Animate a HTML element or SVG element wrapper
        let elem = this;
        // stop regardless of animation actually running, or reverting to
        // .attr (#607)
        uiAnimateStop(elem);
        if (params.d) {
            // keep the array form for paths, used in $.fx.step.d
            elem.toD = params.d;
            // because in jQuery, animating to an array has a different meaning
            params.d = 1;
        }
        uiAnimate(elem, params);
    },
    /**
     * Set or get a given attribute
     * @param {Object|String} hash
     * @param {Mixed|Undefined} val
     */
    attr: function (hash, val) {
        return svgGetset(this, hash, val);
    },
    /**
     * Get the bounding box (width, height, xx and yy) for the element
     */
    getBBox: function () {
        let wrapper = this;
        let bBox = wrapper.bBox;
        let renderer = wrapper.renderer;
        let width;
        let height;
        let rotation = wrapper.rotation;
        let element = wrapper.element;
        let styles = wrapper.styles;
        let rad = rotation * Math.PI * 2 / 360;
        if (!bBox) {
            // SVG elements
            if (element.namespaceURI === "http://www.w3.org/2000/svg") {
                    bBox = element.getBBox();
            }
            // True SVG elements as well as HTML elements in modern browsers using the .useHTML option
            // need to compensated for rotation
            width = bBox.width;
            height = bBox.height;
            // Adjust for rotated text
            if (rotation) {
                bBox.width = height;
                bBox.height = width;
            }
            wrapper.bBox = bBox;
        }
        return bBox;
    },
    /**
     * Add the element
     * @param {Object|Undefined} parent Can be an element, an element wrapper or undefined
     *    to append the element to the renderer.box.
     */
    add: function (parent) {
        let wrapper = this;
        let renderer = wrapper.renderer;
        let parentWrapper = parent || renderer;
        let parentNode = parentWrapper.element || renderer.box;
        let childNodes = parentNode.childNodes;
        let element = wrapper.element;
        let zIndex = attr(element, "zIndex");
        let otherElement;
        let otherZIndex;
        let ii;
        let inserted;
        if (parent) {
            wrapper.parentGroup = parent;
        }
        // build formatted text
        if (wrapper.textStr !== undefined) {
            renderer.buildText(wrapper);
        }
        // mark the container as having z indexed children
        if (zIndex) {
            parentWrapper.handleZ = true;
            zIndex = parseInt(zIndex);
        }
        // insert according to this and other elements' zIndex
        if (parentWrapper.handleZ) { // this element or any of its siblings has a z index
            for (ii = 0; ii < childNodes.length; ii += 1) {
                otherElement = childNodes[ii];
                otherZIndex = attr(otherElement, "zIndex");
                if (otherElement !== element && (
                        // insert before the first element with a higher zIndex
                        parseInt(otherZIndex) > zIndex ||
                        // if no zIndex given, insert before the first element with a zIndex
                        (!isDefined(zIndex) && isDefined(otherZIndex))
                        )) {
                    parentNode.insertBefore(element, otherElement);
                    inserted = true;
                    break;
                }
            }
        }
        // default: append at the end
        if (!inserted) {
            parentNode.appendChild(element);
        }
        // mark as added
        wrapper.added = true;
        // fire an event for internal hooks
        return wrapper;
    }
};
/**
// @param {Object} chart
// @param {Object} options
 */
function Axis() {}
Axis.prototype = {
    translate: function (val, cvsCoord, old) {
        let axis = this;
        let axisLength = axis.len;
        let sign = 1;
        let cvsOffset = 0;
        let localA = old ? axis.oldTransA : axis.transA;
        let localMin = old ? axis.oldMin : axis.min;
        let returnValue;
        if (!localA) {
            localA = axis.transA;
        }
        if (cvsCoord) {
            sign *= -1; // canvas coordinates inverts the value
            cvsOffset = axisLength;
        }
        returnValue = sign * (val - localMin) * localA + cvsOffset + (sign * axis.minPixelPadding);
        return returnValue;
    },
    /**
     * Create the path for a plot line that goes from the given value on
     * this axis, across the plot to the opposite side
     * @param {Number} value
     * @param {Number} lineWidth Used for calculation crisp line
     * @param {Number] old Use old coordinates (for resizing and rescaling)
     */
    getPlotLinePath: function (value, lineWidth, old) {
        let axis = this;
        let chart = axis.chart;
        let axisLeft = axis.left;
        let axisTop = axis.top;
        let x1;
        let y1;
        let x2;
        let y2;
        let translatedValue = axis.translate(value, null, old);
        let cWidth = (old && chart.oldChartWidth) || chart.chartWidth;
        let skip;
        let transB = axis.transB;
        x1 = x2 = Math.round(translatedValue + transB);
        y1 = y2 = Math.round(UI_CHART_HEIGHT - translatedValue - transB);
        if (!Number.isFinite(translatedValue)) { // no min or max
            skip = true;
        } else if (axis.isXAxis) {
            y1 = axisTop;
            y2 = UI_CHART_HEIGHT - axis.bottom;
            if (x1 < axisLeft || x1 > axisLeft + axis.width) {
                skip = true;
            }
        } else {
            x1 = axisLeft;
            x2 = cWidth - axis.right;
            if (y1 < axisTop || y1 > axisTop + axis.height) {
                skip = true;
            }
        }
        return skip ?
            null :
            chart.renderer.crispLine(["M", x1, y1, "L", x2, y2], lineWidth || 0);
    },
    /**
     * Set the tick positions of a linear axis to round values like whole tens or every five.
     */
    getLinearTickPositions: function (tickInterval, min, max) {
        let pos;
        let lastPos;
        let roundedMin = correctFloat(Math.floor(min / tickInterval) * tickInterval);
        let roundedMax = correctFloat(Math.ceil(max / tickInterval) * tickInterval);
        let tickPositions = [];
        // Populate the intermediate values
        pos = roundedMin;
        while (pos <= roundedMax) {
            // Place the tick on the rounded value
            tickPositions.push(pos);
            // Always add the raw tickInterval, not the corrected one.
            pos = correctFloat(pos + tickInterval);
            // If the interval is not big enough in the current min - max range to actually increase
            // the loop variable, we need to break out to prevent endless loop. Issue #619
            if (pos === lastPos) {
                break;
            }
            // Record the last value
            lastPos = pos;
        }
        return tickPositions;
    },
    /**
     * Adjust the min and max for the minimum range. Keep in mind that the series data is
     * not yet processed, so we don't have information on data cropping and grouping, or
     * updated axis.pointRange or series.pointRange. The data can't be processed until
     * we have finally established min and max.
     */
    adjustForMinRange: function () {
        let axis = this;
        let options = axis.options;
        let min = axis.min;
        let max = axis.max;
        let zoomOffset;
        let spaceAvailable = axis.dataMax - axis.dataMin >= axis.minRange;
        let closestDataRange;
        let ii;
        let distance;
        let xData;
        let loopLength;
        let minArgs;
        let maxArgs;
        // Set the automatic minimum range based on the closest point distance
        if (axis.isXAxis && axis.minRange === undefined) {
            // Find the closest distance between raw data points, as opposed to
            // closestPointRange that applies to processed points (cropped and grouped)
            axis.series.forEach(function (series) {
                xData = series.xData;
                loopLength = series.xIncrement ? 1 : xData.length - 1;
                for (ii = loopLength; ii > 0; ii--) {
                    distance = xData[ii] - xData[ii - 1];
                    if (closestDataRange === undefined || distance < closestDataRange) {
                        closestDataRange = distance;
                    }
                }
            });
            axis.minRange = Math.min(closestDataRange * 5, axis.dataMax - axis.dataMin);
        }
        // if minRange is exceeded, adjust
        if (max - min < axis.minRange) {
            let minRange = axis.minRange;
            zoomOffset = (minRange - max + min) / 2;
            // if min and max options have been set, don't go beyond it
            minArgs = [min - zoomOffset, min - zoomOffset];
            if (spaceAvailable) { // if space is available, stay within the data range
                minArgs[2] = axis.dataMin;
            }
            min = arrayMax(minArgs);
            maxArgs = [min + minRange, min + minRange];
            if (spaceAvailable) { // if space is availabe, stay within the data range
                maxArgs[2] = axis.dataMax;
            }
            max = arrayMin(maxArgs);
            // now if the max is adjusted, adjust the min back
            if (max - min < minRange) {
                minArgs[0] = max - minRange;
                minArgs[1] = max - minRange;
                min = arrayMax(minArgs);
            }
        }
        // Record modified extremes
        axis.min = min;
        axis.max = max;
    },
    /**
     * Update translation information
     */
    setAxisTranslation: function () {
        let axis = this;
        let range = axis.max - axis.min;
        let pointRange = 0;
        let closestPointRange;
        let minPointOffset = 0;
        let pointRangePadding = 0;
        let transA = axis.transA;
        // adjust translation for padding
        if (axis.isXAxis) {
            axis.series.forEach(function (series) {
                let seriesPointRange = series.pointRange;
                let seriesClosestPointRange = series.closestPointRange;
                pointRange = Math.max(pointRange, seriesPointRange);
                // minPointOffset is the value padding to the left of the axis in order to make
                // room for points with a pointRange, typically columns. When the pointPlacement option
                // is "between" or "on", this padding does not apply.
                minPointOffset = Math.max(
                    minPointOffset,
                    seriesPointRange / 2
                );
                // Determine the total padding needed to the length of the axis to make room for the
                // pointRange. If the series" pointPlacement is "on", no padding is added.
                pointRangePadding = Math.max(
                    pointRangePadding,
                    seriesPointRange
                );
                // Set the closestPointRange
                if (isDefined(seriesClosestPointRange)) {
                    closestPointRange = isDefined(closestPointRange) ?
                        Math.min(closestPointRange, seriesClosestPointRange) :
                        seriesClosestPointRange;
                }
            });
            // Record minPointOffset and pointRangePadding
            axis.minPointOffset = minPointOffset;
            axis.pointRangePadding = pointRangePadding;
            // pointRange means the width reserved for each point, like in a column chart
            axis.pointRange = pointRange;
            // closestPointRange means the closest distance between points. In columns
            // it is mostly equal to pointRange, but in lines pointRange is 0 while closestPointRange
            // is some other value
            axis.closestPointRange = closestPointRange;
        }
        // secondary values
        axis.oldTransA = transA;
        //axis.translationSlope = axis.transA = transA = axis.len / ((range + (2 * minPointOffset)) || 1);
        axis.translationSlope = axis.transA = transA = axis.len / ((range + pointRangePadding) || 1);
        axis.transB = axis.isXAxis ? axis.left : axis.bottom; // translation addend
        axis.minPixelPadding = transA * minPointOffset;
    },
    /**
     * Update the axis metrics
     */
    setAxisSize: function () {
        let axis = this;
        let chart = axis.chart;
        let options = axis.options;
        let offsetLeft = options.offsetLeft || 0;
        let offsetRight = options.offsetRight || 0;
        // basic values
        // expose to use in Series object and navigator
        axis.left = options.left ?? chart.plotLeft + offsetLeft;
        axis.top = options.top ?? chart.plotTop;
        axis.width = pick(options.width, chart.plotWidth - offsetLeft + offsetRight);
        axis.height = pick(options.height, chart.plotHeight);
        axis.bottom = UI_CHART_HEIGHT - axis.height - axis.top;
        axis.right = chart.chartWidth - axis.width - axis.left;
        axis.len = Math.max(axis.isXAxis ? axis.width : axis.height, 0); // Math.max fixes #905
    },
    /**
     * Position the title
     */
    getTitlePosition: function () {
        // compute anchor points for each of the title align options
        let axis = this;
        let axisLeft = this.left;
        let axisTop = this.top;
        let axisLength = this.len;
        let margin = axis.isXAxis ? axisLeft : axisTop;
        let fontSize = 12;
        // the position in the length direction of the axis
        let alongAxis = margin + axisLength / 2;
        // the position in the perpendicular direction of the axis
        let offAxis = (axis.isXAxis ? axisTop + this.height : axisLeft) +
                (axis.isXAxis ? 1 : -1) * // horizontal axis reverses the margin
                this.axisTitleMargin +
                (this.side === 2 ? fontSize : 0);
        return {
            x: axis.isXAxis ?
                alongAxis :
                offAxis,
            y: axis.isXAxis ?
                offAxis :
                alongAxis
        };
    }
}; // end Axis
/**
// @classDescription The base function which all other series types inherit from. The data in the series is stored
// in various arrays.
 *
// - First, series.options.data contains all the original config options for
// each point whether added by options or methods like series.addPoint.
// - Next, series.data contains those values converted to points, but in case the series data length
// exceeds the cropThreshold, or if the data is grouped, series.data doesn't contain all the points. It
// only contains the points that have been created on demand.
// - Then there's series.points that contains all currently visible point objects. In case of cropping,
// the cropped-away points are not part of this array. The series.points array starts at series.cropStart
// compared to series.data and series.options.data. If however the series data is grouped, these can't
// be correlated one to one.
// - series.xData and series.processedXData contain clean xx values, equivalent to series.data and series.points.
// - series.yData and series.processedYData contain clean xx values, equivalent to series.data and series.points.
 *
// @param {Object} chart
// @param {Object} options
 */
function LineSeries() {}
LineSeries.prototype = {
    type: "line",
    sorted: true, // requires the data to be sorted
    requireSorting: true,
    pointAttrToOptions: { // mapping between SVG attributes and the corresponding options
        stroke: "lineColor",
        "stroke-width": "lineWidth",
        fill: "fillColor",
        r: "radius"
    },
    /**
     * Draw the markers
     */
    drawPoints: function () {
        let series = this;
        let pointAttr;
        let points = series.points;
        let chart = series.chart;
        let plotX;
        let plotY;
        let ii;
        let point;
        let radius;
        let symbol;
        let isImage;
        let graphic;
        let seriesMarkerOptions = series.options.marker;
        let pointMarkerOptions;
        let enabled;
        let isInside;
        let markerGroup = series.markerGroup;
        ii = points.length;
        while (ii--) {
            point = points[ii];
            plotX = point.plotX;
            plotY = point.plotY;
            graphic = point.graphic;
            isInside = chartIsInsidePlot(chart, plotX, plotY);
            // only draw the point if yy is defined
            if (Number.isFinite(plotY)) {
                // shortcuts
                pointAttr = point.pointAttr[""];
                radius = pointAttr.r;
                symbol = series.symbol;
                isImage = symbol.indexOf("url") === 0;
                if (graphic) { // update
                    // Since the marker group isn't clipped, each individual marker must be toggled
                    graphic.element.setAttribute("visibility", (
                        isInside
                        ? "inherit"
                        : "hidden"
                    ));
                    graphic.animate(Object.assign({
                        x: plotX - radius,
                        y: plotY - radius
                    }, graphic.symbolName ? { // don't apply to image symbols #507
                        width: 2 * radius,
                        height: 2 * radius
                    } : {}));
                } else if (isInside && (radius > 0 || isImage)) {
                    point.graphic = graphic = chart.renderer.symbol(
                        symbol,
                        plotX - radius,
                        plotY - radius,
                        2 * radius,
                        2 * radius
                    )
                    .attr(pointAttr);
                    markerGroup.element.appendChild(graphic.element);
                }
            }
        }
    },
    /**
     * Convert state properties from API naming conventions to SVG attributes
     *
     * @param {Object} options API options object
     * @param {Object} base1 SVG attribute object to inherit from
     * @param {Object} base2 Second level SVG attribute object to inherit from
     */
    convertAttribs: function (options, base1, base2, base3) {
        let conversion = this.pointAttrToOptions;
        let attr;
        let option;
        let obj = {};
        options = options || {};
        base1 = base1 || {};
        base2 = base2 || {};
        base3 = base3 || {};
        for (attr in conversion) {
            option = conversion[attr];
            obj[attr] = pick(options[option], base1[attr], base2[attr], base3[attr]);
        }
        return obj;
    },
    /**
     * Get the state attributes. Each series type has its own set of attributes
     * that are allowed to change on a point's state change. Series wide attributes are stored for
     * all series, and additionally point specific attributes are stored for all
     * points with individual marker options. If such options are not defined for the point,
     * a reference to the series wide attributes is stored in point.pointAttr.
     */
    getAttribs: function () {
        let series = this;
        let normalOptions = series.options.marker;
        let stateOptions = normalOptions.states;
        let stateOptionsHover = stateOptions["hover"];
        let pointStateOptionsHover;
        let seriesColor = series.color;
        let normalDefaults = {
                stroke: seriesColor,
                fill: seriesColor
            };
        let points = series.points || []; // #927
        let ii;
        let point;
        let seriesPointAttr = [];
        let pointAttr;
        let pointAttrToOptions = series.pointAttrToOptions;
        let key;
        // series type specific modifications
        if (series.options.marker) { // line, spline, area, areaspline, scatter
            // if no hover radius is given, default to normal radius + 2
            stateOptionsHover.radius = stateOptionsHover.radius || normalOptions.radius + 2;
            stateOptionsHover.lineWidth = stateOptionsHover.lineWidth || normalOptions.lineWidth + 1;
        }
        // general point attributes for the series normal state
        seriesPointAttr[""] = series.convertAttribs(normalOptions, normalDefaults);
        // "hover" and "select" states inherit from normal state except the default radius
        ["hover", "select"].forEach(function (state) {
            seriesPointAttr[state] =
                    series.convertAttribs(stateOptions[state], seriesPointAttr[""]);
        });
        // set it
        series.pointAttr = seriesPointAttr;
        // Generate the point-specific attribute collections if specific point
        // options are given. If not, create a referance to the series wide point
        // attributes
        ii = points.length;
        while (ii--) {
            point = points[ii];
            normalOptions = (point.options && point.options.marker) || point.options;
            if (normalOptions && normalOptions.enabled === false) {
                normalOptions.radius = 0;
            }
            // a specific marker config object is defined for the individual point:
            // no marker config object is created: copy a reference to the series-wide
            // attribute collection
            pointAttr = seriesPointAttr;
            point.pointAttr = pointAttr;
        }
    },
    /**
     * Get the graph path
     */
    getGraphPath: function () {
        let series = this;
        let graphPath = [];
        let singlePoints = []; // used in drawTracker
        // Divide into segments and build graph and area paths
        series.segments.forEach(function (segment) {
            // Return the graph path of a segment
            let segmentPath = [];
            let step = series.options.step;
            // build the segment line
            segment.forEach(function (point, ii) {
                let lastPoint;
                // moveTo or lineTo
                segmentPath.push(ii ? "L" : "M");
                // normal line to next point
                segmentPath.push(
                    point.plotX,
                    point.plotY
                );
            });
            // add the segment to the graph, or a single point for tracking
            if (segment.length > 1) {
                graphPath = graphPath.concat(segmentPath);
            } else {
                singlePoints.push(segment[0]);
            }
        });
        // Record it for use in drawGraph and drawTracker, and return graphPath
        series.singlePoints = singlePoints;
        series.graphPath = graphPath;
        return graphPath;
    },
    /**
     * Draw the actual graph
     */
    drawGraph: function () {
        let series = this;
        let options = series.options;
        let graph = series.graph;
        let group = series.group;
        let color = series.color;
        let attribs;
        let graphPath = series.getGraphPath();
        // draw the graph
        if (graph) {
            graph.animate({ d: graphPath });
            return;
        }
        attribs = {
            stroke: color,
            "stroke-width": 2,
            zIndex: 1 // #1069
        };
        series.graph = series.chart.renderer.path(graphPath)
            .attr(attribs);
        group.element.appendChild(series.graph.element);
        // Add a shadow to the element. Must be done after the element is
        // added to the DOM
        let shadows = [];
        let ii;
        let shadow;
        let element = series.graph.element;
        // compensate for inverted plot area
        for (ii = 1; ii <= 3; ii += 1) {
            shadow = element.cloneNode(0);
            attr(shadow, {
                isShadow: "true",
                stroke: "black",
                "stroke-opacity": 0.05 * ii,
                "stroke-width": 7 - 2 * ii,
                transform: "translate(1, 1)",
                fill: "none"
            });
            element.parentNode.insertBefore(shadow, element);
            shadows.push(shadow);
        }
        series.graph.shadows = shadows;
    },
    /**
     * General abstraction for creating plot groups like series.group, series.trackerGroup, series.dataLabelsGroup and
     * series.markerGroup. On subsequent calls, the group will only be adjusted to the updated plot size.
     */
    plotGroup: function (prop, name, visibility, zIndex, parent) {
        let series = this;
        let group = series[prop];
        let chart = series.chart;
        // Generate it on first call
        if (!group) {
            series[prop] = group = chart.renderer.g(name)
                .attr({
                    visibility: visibility,
                    zIndex: zIndex || 0.1 // IE8 needs this
                });
            parent.element.appendChild(group.element);
        }
        // Place it on first and subsequent (redraw) calls
        group.element.setAttribute("transform", (
            "translate("
            + series.xAxis.left
            + ","
            + series.yAxis.top
            + ")"
        ));
        return group;
    },
    /**
     * Draw the tracker object that sits above all data labels and markers to
     * track mouse events on the graph or points. For the line type charts
     * the tracker uses the same graphPath, but with a greater stroke width
     * for better control.
     */
    drawTracker: function () {
        let series = this;
        let options = series.options;
        let trackerPath = [].concat(series.graphPath);
        let trackerPathLength = trackerPath.length;
        let chart = series.chart;
        let renderer = chart.renderer;
        let tooltipSnap = 10;
        let tracker = series.tracker;
        let cursor = options.cursor;
        let singlePoints = series.singlePoints;
        let singlePoint;
        let ii;
        // Extend end points. A better way would be to use round linecaps,
        // but those are not clickable in VML.
        if (trackerPathLength) {
            ii = trackerPathLength + 1;
            while (ii--) {
                if (trackerPath[ii] === "M") { // extend left side
                    trackerPath.splice(ii + 1, 0, trackerPath[ii + 1] - tooltipSnap, trackerPath[ii + 2], "L");
                }
                if ((ii && trackerPath[ii] === "M") || ii === trackerPathLength) { // extend right side
                    trackerPath.splice(ii, 0, "L", trackerPath[ii - 2] + tooltipSnap, trackerPath[ii - 1]);
                }
            }
        }
        // handle single points
        for (ii = 0; ii < singlePoints.length; ii += 1) {
            singlePoint = singlePoints[ii];
            trackerPath.push("M", singlePoint.plotX - tooltipSnap, singlePoint.plotY,
                "L", singlePoint.plotX + tooltipSnap, singlePoint.plotY);
        }
        // draw the tracker
        if (tracker) {
            svgGetset(tracker, { d: trackerPath });
            return;
        }
        // create
        series.tracker = tracker = renderer.path(trackerPath)
            .attr({
                // invisible but clickable
                stroke: "rgba(192,192,192,0.0001)",
                fill: "none"
            });
        series.chart.elemPlot.querySelector(
            ".uichartTrackerList"
        ).appendChild(tracker.element);
        series.tracker.element.onmouseover = function () {
            series.chart.hoverSeries = series;
        };
    }
}; // end Series prototype
/*jslint-enable*/
/**
// The default SVG renderer
 */
function SVGRenderer() {
    return;
}
function svgCreateElement(renderer, nodeName) {
    let wrapper = new SVGElement();
    wrapper.element = (
        nodeName === "span"
        ? document.createElement("span")
        : document.createElementNS("http://www.w3.org/2000/svg", nodeName)
    );
    wrapper.renderer = renderer;
    // A collection of attribute setters. These methods, if defined, are called
    // right before a certain attribute is set on an element wrapper. Returning
    // false prevents the default attribute setter to run. Returning a value
    // causes the default setter to set that value. Used in Renderer.label.
    wrapper.attrSetters = {};
    return wrapper;
}
SVGRenderer.prototype = {
    /**
     * Create a wrapper for an SVG element
     * @param {Object} nodeName
     */
    /**
     * Parse a simple HTML string into SVG tspans
     *
     * @param {Object} textNode The parent text SVG node
     */
    buildText: function (wrapper) {
        let renderer = this; //jslint-ignore-line
        let textNode = wrapper.element;
        let lines = pick(wrapper.textStr, "").toString().replace((
            /<(b|strong)>/g
        ), `<span style="font-weight:bold">`).replace((
            /<(i|em)>/g
        ), `<span style="font-style:italic">`).replace((
            /<a/g
        ), "<span").replace((
            /<\/(b|strong|i|em|a)>/g
        ), "</span>").split(/<br.*?>/g);
        let childNodes = textNode.childNodes;
        let styleRegex = /style="([^"]+)"/;
        let parentX = attr(textNode, "x");
        let textStyles = wrapper.styles;
        let width = (
            textStyles && textStyles.width && parseInt(textStyles.width)
        );
        let textLineHeight = textStyles && textStyles.lineHeight;
        let lastLine;
        let ii = childNodes.length;
        let linePositions = [];
        // Needed in IE9 because it doesn't report tspan's offsetHeight (#893)
        function getLineHeightByBBox(lineNo) {
            linePositions[lineNo] = (
                textNode.getBBox().height
            ); // #990
            return Math.round(
                linePositions[lineNo] - (linePositions[lineNo - 1] || 0)
            );
        }
        // remove old text
        while (ii > 0) {
            ii -= 1;
            textNode.removeChild(childNodes[ii]);
        }
        if (width && !wrapper.added) {
            // attach it to the DOM to read offset width
            renderer.box.appendChild(textNode);
        }
        // remove empty line at end
        if (lines[lines.length - 1] === "") {
            lines.pop();
        }
        // build the lines
        lines.forEach(function (line, lineNo) {
            let spans;
            let spanNo = 0;
            let lineHeight;
            line = line.replace(/<span/g, "|||<span").replace(/<\/span>/g, "</span>|||");
            spans = line.split("|||");
            spans.forEach(function (span) {
                if (span !== "" || spans.length === 1) {
                    let attributes = {};
                    let tspan = document.createElementNS(
                        "http://www.w3.org/2000/svg",
                        "tspan"
                    );
                    let spanStyle; // #390
                    if (styleRegex.test(span)) {
                        spanStyle = span.match(styleRegex)[1].replace((
                            /(;| |^)color([ :])/m
                        ), "$1fill$2");
                        attr(tspan, "style", spanStyle);
                    }
                    span = String(
                        span.replace(/<(.|\n)*?>/g, "") || " "
                    ).replace(/&lt;/g, "<").replace(/&gt;/g, ">");
                    // add the text node
                    tspan.appendChild(document.createTextNode(span));
                    if (!spanNo) { // first span in a line, align it to the left
                        attributes.x = parentX;
                    // Firefox ignores spaces at the front or end of the tspan
                    } else {
                        attributes.dx = 3; // space
                    }
                    // first span on subsequent line, add the line height
                    if (!spanNo) {
                        if (lineNo) {
                            // Webkit and opera sometimes return "normal" as
                            // the line height. In that case, webkit uses
                            // offsetHeight, while Opera falls back to 18
                            lineHeight = window.getComputedStyle && parseInt(
                                window.getComputedStyle(
                                    lastLine,
                                    null
                                ).getPropertyValue("line-height")
                            );
                            if (!lineHeight || !Number.isFinite(lineHeight)) {
                                lineHeight = (
                                    textLineHeight
                                    || lastLine.offsetHeight
                                    || getLineHeightByBBox(lineNo)
                                    || 18
                                );
                            }
                            attr(tspan, "dy", lineHeight);
                        }
                        lastLine = tspan; // record for use in next line
                    }
                    // add attributes
                    attr(tspan, attributes);
                    // append it
                    textNode.appendChild(tspan);
                    spanNo += 1;
                    // check width and apply soft breaks
                    if (width) {
                        let words = span.replace((
                            /([^\^])-/g
                        ), "$1- ").split(" "); // #1273
                        let tooLong;
                        let actualWidth;
                        let rest = [];
                        while (words.length || rest.length) {
                            delete wrapper.bBox; // delete cache
                            actualWidth = wrapper.getBBox().width;
                            tooLong = actualWidth > width;
                            // new line needed
                            if (!tooLong || words.length === 1) {
                                words = rest;
                                rest = [];
                            }
                        }
                    }
                }
            });
        });
    },
    /**
     * Make a straight line crisper by not spilling out to neighbour pixels
     * @param {Array} points
     * @param {Number} width
     */
    crispLine: function (points, width) {
        // points format: [M, 0, 0, L, 100, 0]
        // normalize to a crisp line
        if (points[1] === points[4]) {
            // Substract due to #1129. Now bottom and left axis gridlines
            // behave the same.
            points[1] = Math.round(points[1]) - (width % 2 / 2);
            points[4] = points[1];
        }
        if (points[2] === points[5]) {
            points[2] = Math.round(points[2]) + (width % 2 / 2);
            points[5] = points[2];
        }
        return points;
    },
    /**
     * Draw a path
     * @param {Array} path An SVG path in array form
     */
    path: function (path) {
        let renderer = this; //jslint-ignore-line
        let __attr = {
            fill: "none"
        };
        if (Array.isArray(path)) {
            __attr.d = path;
        }
        return svgGetset(svgCreateElement(renderer, "path"), __attr);
    },
    /**
     * Create a group
     */
    g: function (name) {
        let renderer = this; //jslint-ignore-line
        let elem = svgCreateElement(renderer, "g");
        return (
            isDefined(name)
            ? svgGetset(elem, {class: PREFIX + name})
            : elem
        );
    },
    symbol: function (symbol, xx, yy, width, height, options) {
    // Draw a symbol out of pre-defined shape paths from the namespace
    // "symbol" object.
        let renderer = this; //jslint-ignore-line
        let obj;
        // get the symbol definition function
        let symbolFn = renderer.symbols[symbol];
        // check if there's a path defined for this symbol
        let path = symbolFn(
            Math.round(xx),
            Math.round(yy),
            width,
            height,
            options
        );
        obj = renderer.path(path);
        // expando properties for use in animate and attr
        Object.assign(obj, {
            height,
            symbolName: symbol,
            width,
            x: xx,
            y: yy
        });
        return obj;
    },
    /**
     * An extendable collection of functions for defining symbol paths.
     */
    symbols: {
        circle: function (xx, yy, ww, hh) {
            let cpw = 0.166 * ww;
            return [
                "M", xx + ww / 2, yy,
                "C", xx + ww + cpw, yy, xx + ww + cpw, yy + hh, xx + ww / 2,
                yy + hh,
                "C", xx - cpw, yy + hh, xx - cpw, yy, xx + ww / 2, yy,
                "Z"
            ];
        },
        square: function (xx, yy, ww, hh) {
            return [
                "M", xx, yy,
                "L", xx + ww, yy,
                xx + ww, yy + hh,
                xx, yy + hh,
                "Z"
            ];
        },
        triangle: function (xx, yy, ww, hh) {
            return [
                "M", xx + ww / 2, yy,
                "L", xx + ww, yy + hh,
                xx, yy + hh,
                "Z"
            ];
        },
        "triangle-down": function (xx, yy, ww, hh) {
            return [
                "M", xx, yy,
                "L", xx + ww, yy,
                xx + ww / 2, yy + hh,
                "Z"
            ];
        },
        diamond: function (xx, yy, ww, hh) {
            return [
                "M", xx + ww / 2, yy,
                "L", xx + ww, yy + hh / 2,
                xx + ww / 2, yy + hh,
                xx, yy + hh / 2,
                "Z"
            ];
        }
    },
    color: function (color, elem, prop) {
    // Take a color and return it if it's a string, make it a gradient if it's
    // a gradient configuration object. Prior to Highstock, an array was used
    // to define a linear gradient with pixel positions relative to the SVG. In
    // newer versions we change the coordinates to apply relative to the shape,
    // using coordinates 0-1 within the shape. To preserve backwards
    // compatibility, linearGradient in this definition is an object
    // of x1, y1, x2 and y2.
        let renderer = this; //jslint-ignore-line
        // Remove the opacity attribute added above. Does not throw if the
        // attribute is not there.
        elem.removeAttribute(prop + "-opacity");
        return color;
    },
    /**
     * Add text to the SVG object
     */
    text: function (str, xx, yy) {
        // declare variables
        let renderer = this; //jslint-ignore-line
        let wrapper;
        xx = Math.round(pick(xx, 0));
        yy = Math.round(pick(yy, 0));
        wrapper = svgCreateElement(renderer, "text").attr({
            x: xx,
            y: yy,
            text: str
        });
        wrapper.x = xx;
        wrapper.y = yy;
        return wrapper;
    },
    fontMetrics: function (fontSize) {
    // Utility to return the baseline offset and total line height from the
    // font size
        fontSize = parseInt(fontSize || 11);
        // Empirical values found by comparing font size and bounding box
        // height. Applies to the default font family.
        // http://jsfiddle.net/highcharts/7xvn7/
        let lineHeight = (
            fontSize < 24
            ? fontSize + 4
            : Math.round(fontSize * 1.2)
        );
        return {
            h: lineHeight,
            b: Math.round(lineHeight * 0.8)
        };
    }
}; // end SVGRenderer
function arrayMin(data) {
// this function will find min in <data>
    let ii = data.length;
    let min = data[0];
    while (ii > 0) {
        ii -= 1;
        min = Math.min(min, data[ii]);
    }
    return min;
}
function arrayMax(data) {
// this function will find max in <data>
    let ii = data.length;
    let max = data[0];
    while (ii > 0) {
        ii -= 1;
        max = Math.max(max, data[ii]);
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
function chartGetMargins(chart) {
// Calculate margins by rendering axis labels in a preliminary position. Title,
// subtitle and legend have already been rendered at this stage, but will be
// moved into their final positions
    // Initial margins before auto size margins are applied
    chart.plotTop = 10;
    chart.marginRight = 10;
    chart.marginBottom = 15;
    chart.plotLeft = 10;
    // top, right, bottom, left
    chart.axisOffset = [0, 0, 0, 0];
    // adjust for title and subtitle
    chart.plotTop = Math.max(
        chart.plotTop,
        15 + 15 + 10
    );
    // pre-render axes to get labels offset width
    chart.axisList.forEach(function (axis, ii) {
        // Render the tick labels to a preliminary position to get their sizes
        let axisOffset = chart.axisOffset;
        let {
            categoryList,
            isXAxis,
            tickDict,
            tickPositions
        } = axis;
        let directionFactor = noop([-1, 1, 1, -1])[axis.side];
        let labelOffset = 0; // reset
        let renderer = chart.renderer;
        let titleMargin = 0;
        let titleOffset = 0;
        // Create the axisGroup and gridGroup elements on first iteration
        if (!axis.axisGroup) {
            axis.axisGroup = renderer.g("axis");
            axis.axisGroup.element = chart.elemPlot.querySelector(
                `.uichartAxis${ii}`
            );
            axis.gridGroup = renderer.g("grid");
            axis.gridGroup.element = chart.elemPlot.querySelector(
                `.uichartGrid${ii}`
            );
            axis.labelGroup = renderer.g("axis-labels");
            axis.labelGroup.element = chart.elemPlot.querySelector(
                `.uichartAxisLabel${ii}`
            );
        }
        Array.from(tickPositions || []).forEach(function (pos) {
            let label;
            let tick;
            tickDict[pos] = tickDict[pos] || {
                axis,
                isNew: true,
                pos
            };
            tick = tickDict[pos];
            label = tick.label;
            (function tickLabelCreateOrUpdate() {
                let str;
                let width;
                width = (
                    categoryList.length
                    ? chart.plotWidth / tickPositions.length
                    : isXAxis
                    ? 0
                    : 0.5 * chart.plotWidth
                );
                width = width && `${Math.round(Math.max(1, width - 20))}px`;
                // tickLabelUpdate
                if (label) {
                    return;
                }
                // tickLabelCreate
                // set properties for access in render method
                // get the string
                // The default label formatter.
                str = (function tickLabelFormatter() {
                    let val = categoryList[pos] ?? pos;
                    let val2;
                    if (typeof val !== "number") {
                        return val;
                    }
                    // small number
                    if (Math.abs(val) < 1000) {
                        return val.toLocaleString();
                    }
                    // large number
                    val = Math.round(val).toLocaleString();
                    val2 = val.split(
                        /[^+\-0-9]/
                    );
                    switch (val2.length) {
                    case 2:
                        return val2[0] + "k";
                    case 3:
                        return val2[0] + "M";
                    case 4:
                        return val2[0] + "G";
                    case 5:
                        return val2[0] + "T";
                    case 6:
                        return val2[0] + "P";
                    case 7:
                        return val2[0] + "E";
                    default:
                        return val;
                    }
                }());
                label = chart.renderer.text(str, 0, 0);
                label.element.setAttribute("text-anchor", (
                    isXAxis
                    ? "middle"
                    : "end"
                ));
                label.element.setAttribute(
                    "style",
                    "color:#666;fill:#666;font-size:11px;line-height:14px;"
                );
                if (width) {
                    label.element.style.width = width;
                }
                axis.labelGroup.element.appendChild(label.element);
                label.add(axis.labelGroup);
                tick.label = label;
            }());
            // left side must be align: right and right side
            // must have align: left for labels
            // get the highest offset
            // Get the offset height or width of the label
            tick.labelBBox = label.getBBox();
            labelOffset = Math.max(
                labelOffset,
                tick.labelBBox[(
                    isXAxis
                    ? "height"
                    : "width"
                )]
            );
        });
        // doesn't have data
        if (
            !axis.hasVisibleSeries
            && !(
                isDefined(axis.min)
                && isDefined(axis.max)
                && tickPositions
            )
        ) {
            Object.entries(tickDict).forEach(function ([
                key, tick
            ]) {
                destroyObjectProperties(tick, tick.axis);
                delete tickDict[key];
            });
        }
        if (!axis.axisTitle) {
            axis.axisTitle = renderer.text(
                axis.options.title.text,
                0,
                0
            );
            axis.axisTitle.element.setAttribute("class", "uichartLabelAxis");
            axis.axisTitle.element.setAttribute("zIndex", "7");
            axis.axisTitle.element.setAttribute("text-anchor", "middle");
            if (!isXAxis) {
                svgGetset(axis.axisTitle, {
                    rotation: 270
                });
            }
            axis.axisTitle.add(axis.axisGroup);
            axis.axisTitle.isNew = true;
        }
        titleOffset = axis.axisTitle.getBBox()[(
            isXAxis
            ? "height"
            : "width"
        )];
        titleMargin = (
            isXAxis
            ? 5
            : 10
        );
        axis.axisTitleMargin = (
            labelOffset
            + titleMargin
            + (
                axis.side !== 2
                && labelOffset
                && directionFactor * (
                    isXAxis
                    ? 15
                    : -8
                )
            )
        );
        axisOffset[axis.side] = Math.max(
            axisOffset[axis.side],
            axis.axisTitleMargin + titleOffset
        );
    });
    // end chart.axisList.forEach()
    chart.marginBottom += chart.axisOffset[2];
    chart.marginRight += chart.axisOffset[1];
    chart.plotLeft += chart.axisOffset[3];
    chart.plotTop += chart.axisOffset[0];
    // Set the public chart properties. This is done before and after the
    // pre-render to determine margin sizes
    chart.plotLeft = Math.round(chart.plotLeft);
    chart.plotTop = Math.round(chart.plotTop);
    chart.plotWidth = Math.max(
        0,
        Math.round(chart.chartWidth - chart.plotLeft - chart.marginRight)
    );
    chart.plotHeight = Math.max(
        0,
        Math.round(UI_CHART_HEIGHT - chart.plotTop - chart.marginBottom)
    );
    chart.plotSizeX = chart.plotWidth;
    chart.plotSizeY = chart.plotHeight;
    chart.plotBorderWidth = chart.options.plotBorderWidth || 0;
    // Set boxes used for alignment
    chart.spacingBox = {
        height: UI_CHART_HEIGHT - 10 - 15,
        width: chart.chartWidth - 10 - 10,
        x: 10,
        y: 10
    };
    chart.plotBox = {
        height: chart.plotHeight,
        width: chart.plotWidth,
        x: chart.plotLeft,
        y: chart.plotTop
    };
    chart.clipBox = {
        height: chart.plotSizeY - chart.plotBorderWidth,
        width: chart.plotSizeX - chart.plotBorderWidth,
        x: chart.plotBorderWidth / 2,
        y: chart.plotBorderWidth / 2
    };
    chart.axisList.forEach(function (axis) {
        axis.setAxisSize();
        axis.setAxisTranslation();
    });
}
function chartIsInsidePlot({
    plotHeight,
    plotWidth
}, xx, yy) {
// Check whether a given point is within the plot area
    return 0 <= xx && xx <= plotWidth && 0 <= yy && yy <= plotHeight;
}
function uichartOnAction(evt) {
// this function will reset zoom of uichart
    let chart;
    let chartX;
    let {
        currentTarget,
        target
    } = evt;
    let series;
    let xAxis;
    let zoomDelta;
    let zoomMid;
    if (!evt.modeDebounce) {
        debounce(`uichartOnAction.${evt.type}`, function () {
            evt.modeDebounce = true;
            uichartOnAction(evt);
        });
        return;
    }
    switch (evt.type) {
    case "resize":
        DBCHART_DICT.forEach(function (chart) {
            let chartWidth = Math.round(
                -UI_CHART_LEGEND_WIDTH + chart.renderTo.clientWidth
            );
            let {
                container,
                renderer
            } = chart;
            if (chartWidth === chart.containerWidth) {
                return;
            }
            // Resize the chart to a given width and height
            chart.chartWidth = chartWidth;
            container.style.width = `${chart.chartWidth}px`;
            // Resize the box and re-align all aligned elements
            renderer.boxWrapper.animate({
                height: UI_CHART_HEIGHT,
                width: chartWidth
            });
            // update axis lengths for more correct tick intervals:
            chart.plotWidth = chartWidth - chart.plotLeft - chart.marginRight;
            chart.plotHeight = (
                UI_CHART_HEIGHT - chart.plotTop - chart.marginBottom
            );
            // handle axes
            chart.maxTicks = null;
            // uichartRedraw - resize
            uichartRedraw(chart);
            // move titles
            if (chart.elemTitle) {
                svgAlign({
                    box: chart.spacingBox,
                    svgWrapper: chart.elemTitle
                });
            }
            chart.containerWidth = chartWidth;
        });
        return;
    // zoom in/out on wheelup/wheeldown respectively
    case "wheel":
        if (!currentTarget) {
            return;
        }
        chart = DBCHART_DICT.get(currentTarget.closest(".uichartDiv").id);
        chartX = uichartXY(chart, evt)[0];
        xAxis = chart.axisList[0];
        zoomDelta = (
            evt.deltaY < 0
            ? 0.75
            : 1.25
        ) * 0.5 * (xAxis.dataMax - xAxis.dataMin);
        zoomMid = (
            xAxis.dataMin
            + (chartX / chart.plotWidth) * (xAxis.dataMax - xAxis.dataMin)
        );
        xAxis.userMin = zoomMid - zoomDelta;
        xAxis.userMax = zoomMid + zoomDelta;
        // uichartRedraw - zoomWheel
        uichartRedraw(chart);
        return;
    }
    target = target.closest(".uichartAction");
    if (!target) {
        return;
    }
    evt.preventDefault();
    evt.stopPropagation();
    chart = DBCHART_DICT.get(target.closest(".uichartDiv").id);
    switch (target.dataset.action) {
    case "seriesHideAll":
        chart.seriesList.forEach(function (series) {
            series.visible = false;
            uichartSeriesHideOrShow({
                series
            });
        });
        chart.container.querySelectorAll(
            ".uichartLegendElem"
        ).forEach(function (elem) {
            elem.dataset.hidden = 1;
        });
        // uichartRedraw - seriesHideAll
        uichartRedraw(chart);
        return;
    case "seriesShowAll":
        chart.seriesList.forEach(function (series) {
            series.visible = true;
            uichartSeriesHideOrShow({
                series
            });
        });
        chart.container.querySelectorAll(
            ".uichartLegendElem"
        ).forEach(function (elem) {
            elem.dataset.hidden = 0;
        });
        // uichartRedraw - seriesShowAll
        uichartRedraw(chart);
        return;
    case "seriesVisibilityToggle":
        series = chart.seriesList[target.dataset.seriesIi];
        series.visible = !series.visible;
        uichartSeriesHideOrShow({
            series
        });
        target.dataset.hidden = target.dataset.hidden ^ 1;
        // uichartRedraw - seriesVisibility
        uichartRedraw(chart);
        return;
    case "zoomReset":
        chart.axisList.forEach(function (axis) {
            delete axis.userMin;
            delete axis.userMax;
        });
        // uichartRedraw - zoomReset
        uichartRedraw(chart);
        return;
    }
    throw new Error(`invalid action ${evt.type}.${target.dataset.action}`);
}
function uichartRedraw(chart) {
/**
// Redraw legend, axes or series based on updated data
 *
// @param {Boolean|Object} animation Whether to apply animation,
// and optionally animation configuration
 */
    let {
        elemClip,
        renderer
    } = chart;
    // handle added or removed series
    // series or pie points are added or removed
    // reset maxTicks
    chart.maxTicks = null;
    // set axes scales
    // Set the scale based on data min and max, user set min and max or options
    chart.axisList.forEach(function (axis) {
        let {
            tickInterval
        } = axis;
        let {
            endOnTick,
            maxPadding,
            minPadding,
            startOnTick,
            tickPixelInterval
        } = axis.options;
        let ii;
        axis.oldMin = axis.min;
        axis.oldMax = axis.max;
        axis.oldAxisLength = axis.len;
        // set the new axisLength
        axis.setAxisSize();
        // do we really need to go through all this?
        // get data extremes if needed
        //  Get the minimum and maximum for the series of each axis
        axis.hasVisibleSeries = false;
        // reset dataMin and dataMax in case we're redrawing
        axis.dataMax = null;
        axis.dataMin = null;
        // loop through this axis" series
        axis.series.forEach(function (series) {
            if (!series.visible) {
                return;
            }
            let xData;
            let yData;
            let xx;
            let activeYData = [];
            let activeCounter = 0;
            axis.hasVisibleSeries = true;
            // Get dataMin and dataMax for X axes
            if (axis.isXAxis) {
                xData = series.xData;
                if (xData.length) {
                    axis.dataMin = Math.min(
                        pick(axis.dataMin, xData[0]),
                        arrayMin(xData)
                    );
                    axis.dataMax = Math.max(
                        pick(axis.dataMax, xData[0]),
                        arrayMax(xData)
                    );
                }
                return;
            }
            // Get dataMin and dataMax for Y axes, as well as handle stacking
            // and processed data
            let cropped = series.cropped;
            //findPointRange
            //pointRange
            // processData can alter series.pointRange, so this goes after
            //findPointRange = series.pointRange === null;
            xData = series.xData;
            yData = series.yData;
            // loop over the non-null yy values and read them into a local array
            yData.forEach(function (yy, ii) {
                xx = xData[ii];
                // Handle non null values
                if (yy !== null && yy !== undefined) {
                    // for points within the visible range, including the first
                    // point outside the visible range, consider yy extremes
                    if (cropped || (
                        (xData[ii + 1] || xx) >= series.xAxis.min
                        && (xData[ii - 1] || xx) <= series.xAxis.max
                    )) {
                        activeYData[activeCounter] = yy;
                        activeCounter += 1;
                    }
                }
            });
            // Get the dataMin and dataMax so far. If percentage is used, the
            // min and max are always 0 and 100. If the length of activeYData
            // is 0, continue with null values.
            if (activeYData.length) {
                axis.dataMin = Math.min(
                    pick(axis.dataMin, activeYData[0]),
                    arrayMin(activeYData)
                );
                axis.dataMax = Math.max(
                    pick(axis.dataMax, activeYData[0]),
                    arrayMax(activeYData)
                );
            }
        });
        //
        // get fixed positions based on tickInterval
        // Set the tick positions to round values and optionally extend the
        // extremes to the nearest tick
        let isXAxis = axis.isXAxis;
        let magnitude;
        let length;
        let categoryList = axis.categoryList;
        // initial min and max from the extreme data values
        axis.min = axis.userMin ?? axis.dataMin;
        axis.max = axis.userMax ?? axis.dataMax;
        // adjust min and max for the minimum range
        axis.adjustForMinRange();
        // Pad the values to get clear of the chart's edges. To avoid
        // tickInterval taking the padding into account, we do this after
        // computing tick interval (#1337).
        if (
            !categoryList.length && isDefined(axis.min)
            && isDefined(axis.max)
        ) {
            length = axis.max - axis.min;
            if (length) {
                if (
                    !isDefined(axis.userMin)
                    && minPadding
                    && (axis.dataMin < 0 || !axis.ignoreMinPadding)
                ) {
                    axis.min -= length * minPadding;
                }
                if (
                    !isDefined(axis.userMax)
                    && maxPadding
                    && (axis.dataMax > 0 || !axis.ignoreMaxPadding)
                ) {
                    axis.max += length * maxPadding;
                }
            }
        }
        tickInterval = (
            categoryList.length
            ? 1
            : (
                (axis.max - axis.min)
                * tickPixelInterval
                / (axis.len || 1)
            )
        );
        // Now we're finished detecting min and max, crop and group series data.
        // This is in turn needed in order to find tick positions in ordinal
        // axes.
        axis.series.forEach(function (series) {
            if (!isXAxis) {
                return;
            }
            // copied during slice operation below
            let closestPointRange = 0;
            // Return false to pass Find the closest distance between processed
            ii = series.xData.length - 1;
            while (ii > 0) {
                ii -= 1;
                closestPointRange = Math.min(
                    closestPointRange,
                    Math.max(0, series.xData[ii] - series.xData[ii + 1])
                );
            }
            // null means auto, as for columns, candlesticks and OHLC
            series.pointRange = closestPointRange || 1;
            series.closestPointRange = closestPointRange;
        });
        // set the translation factor used in translate function
        axis.setAxisTranslation();
        // for linear axes, get magnitude and normalize the interval
        magnitude = Math.pow(
            10,
            Math.floor(Math.log(tickInterval) / Math.LN10)
        );
        // Take an interval and normalize it to multiples of 1, 2, 2.5 and 5
        let normalized;
        // round to a tenfold of 1, 2, 2.5 or 5
        magnitude = magnitude ?? 1;
        normalized = tickInterval / magnitude;
        let multiples = [1, 2, 2.5, 5, 10];
        // normalize the interval to the nearest multiple
        ii = 0;
        while (ii < multiples.length) {
            tickInterval = multiples[ii];
            if (
                normalized <= (
                    multiples[ii]
                    + (multiples[ii + 1] || multiples[ii])
                ) / 2
            ) {
                break;
            }
            ii += 1;
        }
        // multiply back to the correct magnitude
        tickInterval *= magnitude;
        // find the tick positions
        axis.tickPositions = axis.getLinearTickPositions(
            tickInterval,
            axis.min,
            axis.max
        );
        // reset min/max or remove extremes based on start/end on tick
        let roundedMin = axis.tickPositions[0];
        let roundedMax = axis.tickPositions[axis.tickPositions.length - 1];
        let minPointOffset = axis.minPointOffset || 0;
        let singlePad;
        if (startOnTick) {
            axis.min = roundedMin;
        } else if (axis.min - minPointOffset > roundedMin) {
            axis.tickPositions.shift();
        }
        if (endOnTick) {
            axis.max = roundedMax;
        } else if (axis.max + minPointOffset < roundedMax) {
            axis.tickPositions.pop();
        }
        // When there is only one point, or all points have the same value on
        // this axis, then min and max are equal and tickPositions.length is 1.
        // In this case, add some padding in order to center the point, but
        // leave it with one tick. #1337.
        if (axis.tickPositions.length === 1) {
            // The lowest possible number to avoid extra padding on columns
            singlePad = 1e-9;
            axis.min -= singlePad;
            axis.max += singlePad;
        }
        //
        // record old values to decide whether a rescale is necessary later on
        axis.oldUserMin = axis.userMin;
        axis.oldUserMax = axis.userMax;
        // Set the maximum tick amount
        // Set the max ticks of either the x and y axis collection
        let maxTicks = chart.maxTicks;
        let xOrY = axis.xOrY;
        // first call, or maxTicks have been reset after a zoom operation
        if (!maxTicks) {
            maxTicks = {
                x: 0,
                y: 0
            };
        }
        if (
            axis.tickPositions.length > maxTicks[xOrY]
        ) {
            maxTicks[xOrY] = axis.tickPositions.length;
        }
        chart.maxTicks = maxTicks;
        axis.tickInterval = tickInterval;
    });
    // Adjust all axes tick amounts
    // When using multiple axes, adjust the number of ticks to match the
    // highest number of ticks in that group
    chart.axisList.forEach(function (axis) {
        let xOrY = axis.xOrY;
        let tickPositions = axis.tickPositions;
        let maxTicks = chart.maxTicks;
        // only apply to linear scale
        let calculatedTickAmount = tickPositions.length;
        // set the axis-level tickAmount to use below
        let tickAmount = maxTicks[xOrY];
        if (calculatedTickAmount < tickAmount) {
            while (tickPositions.length < tickAmount) {
                tickPositions.push(correctFloat(
                    tickPositions[tickPositions.length - 1] + axis.tickInterval
                ));
            }
            axis.transA *= (calculatedTickAmount - 1) / (tickAmount - 1);
            axis.max = tickPositions[tickPositions.length - 1];
        }
    });
    chart.maxTicks = null;
    chartGetMargins(chart);
    // redraw axes
    chart.axisList.forEach(function (axis) {
        let {
            axisTitle,
            isXAxis,
            tickDict,
            tickPositions
        } = axis;
        // render the axis
        let lineWidth = axis.options.lineWidth;
        let linePath;
        // If the series has data draw the ticks. Else only the line and title
        // Major ticks. Pull out the first item and render it last so that
        // we can get the position of the neighbour label. #808.
        // start if hasData
        tickPositions.slice(1).concat(
            tickPositions[0]
        ).forEach(function (pos, tickIi) {
            let tick = tickDict[pos];
            if (!axis.hasVisibleSeries) {
                return;
            }
            // Reorganize the indices
            tickIi = (
                (tickIi === tickPositions.length - 1)
                ? 0
                : tickIi + 1
            );
            tickDict[pos].isActive = true;
            // tick - Put everything in place
            let label = tick.label;
            let {
                tickWidth
            } = axis.options;
            let gridLinePath;
            let mark = tick.mark;
            let markPath;
            let attribs;
            let tickmarkOffset = axis.tickmarkOffset;
            // Get the x and y position for ticks and labels
            label.xy = {
                x: (
                    isXAxis
                    ? (
                        axis.translate(pos + tickmarkOffset, null)
                        + axis.transB
                    )
                    : axis.left
                ),
                y: (
                    isXAxis
                    ? UI_CHART_HEIGHT - axis.bottom
                    : (
                        UI_CHART_HEIGHT
                        - axis.translate(pos + tickmarkOffset, null)
                        - axis.transB
                    )
                )
            };
            //
            let xx = label.xy.x;
            let yy = label.xy.y;
            // create the grid line
            if (!isXAxis) {
                gridLinePath = axis.getPlotLinePath(
                    pos + tickmarkOffset,
                    1
                );
                if (tick.gridLine === undefined) {
                    attribs = {
                        stroke: "#C0C0C0",
                        "stroke-width": 1
                    };
                    attribs.zIndex = 1;
                    tick.gridLine = renderer.path(
                        gridLinePath
                    ).attr(attribs);
                    axis.gridGroup.element.appendChild(tick.gridLine.element);
                }
                // If the parameter "old" is set, the current call will be
                // followed by another call, therefore do not do any animations
                // this time
                if (tick.gridLine && gridLinePath) {
                    tick.gridLine[(
                        tick.isNew
                        ? "attr"
                        : "animate"
                    )]({
                        d: gridLinePath
                    });
                }
            }
            // create the tick mark
            if (tickWidth) {
                markPath = renderer.crispLine([
                    "M",
                    xx,
                    yy,
                    "L",
                    xx + (
                        isXAxis
                        ? 0
                        : -5
                    ),
                    yy + (
                        isXAxis
                        ? 5
                        : 0
                    )
                ], tickWidth);
                if (mark) { // updating
                    // animate tickmark
                    mark.animate({
                        d: markPath
                    });
                } else { // first time
                    tick.mark = renderer.path(
                        markPath
                    ).attr({
                        stroke: "#C0D0E0",
                        "stroke-width": tickWidth
                    }).add(axis.axisGroup);
                }
            }
            // the label is created on init - now move it into place
            if (label && Number.isFinite(xx)) {
                xx = xx + (
                    isXAxis
                    ? 0
                    : -8
                ) - (
                    (tickmarkOffset && isXAxis)
                    ? tickmarkOffset * axis.transA
                    : 0
                );
                yy = yy + (
                    isXAxis
                    ? 15
                    : 3
                ) - (
                    (tickmarkOffset && !isXAxis)
                    ? tickmarkOffset * -axis.transA
                    : 0
                );
                label.xy = {
                    x: xx,
                    y: yy
                };
                // Set the new position, and show or hide
                label[(
                    tick.isNew
                    ? "attr"
                    : "animate"
                )](label.xy);
                tick.isNew = false;
            }
        });
        // end if hasData
        // remove inactive ticks
        Object.entries(tickDict).forEach(function ([
            key, tick
        ]) {
            if (!tick.isActive) {
                destroyObjectProperties(tick, tick.axis);
                delete tickDict[key];
            } else {
                tick.isActive = false; // reset
            }
        });
        // Static items. As the axis group is cleared on subsequent calls
        // to render, these items are added outside the group.
        // axis line
        if (lineWidth) {
            // Get the path for the axis line
            linePath = chart.renderer.crispLine([
                "M",
                axis.left,
                (
                    isXAxis
                    ? UI_CHART_HEIGHT - axis.bottom
                    : axis.top
                ),
                "L",
                (
                    isXAxis
                    ? chart.chartWidth - axis.right
                    : axis.left
                ),
                UI_CHART_HEIGHT - axis.bottom
            ], lineWidth);
            if (!axis.axisLine) {
                axis.axisLine = renderer.path(linePath).attr({
                    stroke: axis.options.lineColor,
                    "stroke-width": lineWidth,
                    zIndex: 7
                }).add(axis.axisGroup);
            } else {
                axis.axisLine.animate({d: linePath});
            }
        }
        if (axisTitle) {
            axisTitle[(
                axisTitle.isNew
                ? "attr"
                : "animate"
            )](axis.getTitlePosition());
            axisTitle.isNew = false;
        }
    });
    // the plot areas size has changed
    // Draw the borders and backgrounds for chart and plot area
    // Chart area
    // Plot clip
    elemClip.animate({
        height: chart.clipBox.height,
        width: chart.clipBox.width
    });
    // redraw affected series
    chart.seriesList.forEach(function (series) {
        let {
            xAxis
        } = series;
        if (!series.visible) { // issue #153
            return;
        }
        // Translate data points from raw data values to chart specific
        // positioning data needed later in drawPoints, drawGraph and
        // drawTracker.
        // Generate the data point after the data has been processed by
        // cropping away unused points
        series.points = series.points || [];
        series.xData.forEach(function (xx, ii) {
            let point = series.points[ii] || {
                pointAttr: {},
                series,
                x: xx,
                // onedimensional array input
                y: series.options.data[ii]
            };
            series.points[ii] = point;
            // Translate each point
            // get the plotX translation
            // Math.round fixes #591
            point.plotX = xAxis.translate(point.x, 0, 0);
            // Set the the plotY value, reset it for redraws
            point.plotY = (
                typeof point.y === "number"
                ? 0.1 * Math.round(
                    10 * series.yAxis.translate(point.y, 1, 0)
                )
                // Math.round fixes #591
                : undefined
            );
            // some API data
            point.category = (
                xAxis.categoryList[point.x] !== undefined
                ? xAxis.categoryList[point.x]
                : point.x
            );
        });
        // now that we have the cropped data, build the segments
        // Divide the series data into segments divided by null values.
        let lastNull = -1;
        let segments = [];
        let points = series.points;
        let pointsLength = points.length;
        if (pointsLength) { // no action required for []
            points.forEach(function (point, ii) {
                if (point.y === null) {
                    if (ii > lastNull + 1) {
                        segments.push(points.slice(lastNull + 1, ii));
                    }
                    lastNull = ii;
                } else if (ii === pointsLength - 1) { // last value
                    segments.push(points.slice(lastNull + 1, ii + 1));
                }
            });
        }
        // register it
        series.segments = segments;
        //
        // Memoize tooltip texts and positions
        let axisLength = (
            xAxis
            ? xAxis.len
            : series.chart.plotSizeX
        );
        let low;
        let high;
        let plotX = (xAxis && xAxis.tooltipPosName) || "plotX";
        // a lookup array for each pixel in the x dimension
        let tooltipPoints = [];
        // concat segments to overcome null values
        series.segments.forEach(function (segment) {
            points = points.concat(segment);
        });
        // Assign each pixel position to the nearest point
        series.segments.flat().forEach(function (point, ii, list) {
            // Set this range's low to the last range's high plus one
            low = (
                list[ii - 1]
                ? high + 1
                : 0
            );
            // Now find the new high
            high = (
                list[ii + 1]
                ? Math.max(
                    0,
                    Math.floor((point[plotX] + (
                        list[ii + 1]
                        ? list[ii + 1][plotX]
                        : axisLength
                    )) / 2)
                )
                : axisLength
            );
            while (0 <= low && low <= high) {
                tooltipPoints[low] = point;
                low += 1;
            }
        });
        series.tooltipPoints = tooltipPoints;
        //
        // Render the graph and markers
        let visibility = (
            series.visible
            ? "visible"
            : "hidden"
        );
        // the group
        let group = series.plotGroup(
            "group",
            "series",
            visibility,
            series.options.zIndex,
            chart.elemSeriesList
        );
        series.markerGroup = series.plotGroup(
            "markerGroup",
            "markers",
            visibility,
            series.options.zIndex,
            chart.elemSeriesList
        );
        // cache attributes for shapes
        series.getAttribs();
        // draw the graph if any
        if (series.drawGraph) {
            series.drawGraph();
        }
        // draw the points
        series.drawPoints();
        // draw the mouse tracking area
        series.drawTracker();
        // Initial clipping, must be defined after inverting groups for VML
        if (
            series.options.clip !== false
            && !series.hasRendered
        ) {
            // Apply a clipping path to this object
            svgGetset(group, "clip-path", (
                "url(" + renderer.url + "#" + elemClip.id + ")"
            ));
        }
        // means data is in accordance with what you see
        series.hasRendered = true;
    });
    // move tooltip or reset
    // Redraw the axis to reflect changes in the data or axis extremes
    // hide tooltip and hover states
    uichartTooltipHide(chart);
}
function debounce(key, func) {
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
            val.func();
        }, 250)
    };
    DEBOUNCE_DICT[key] = val;
    // if first-time, then immediately call <func>
    func();
}
function domDivCreate(innerHTML) {
// this function will return div-element with rendered <innerHTML>
    let elem = document.createElement("div");
    elem.innerHTML = innerHTML;
    return elem;
}
function isDefined(obj) {
// Returns true if the object is not null or undefined.
    return obj !== undefined && obj !== null;
}
function noop(val) {

// This function will do nothing except return <val>.

    return val;
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
function svgAlign({
    alignOptions,
    box,
    svgWrapper
}) {
/**
// Break down alignment options like align, verticalAlign, x and y
// to x and y relative to the chart.
 *
// @param {Object} alignOptions
// @param {Boolean} alignByTranslate
// @param {Object} box The box to align to, needs a width and height
 *
 */
    let {
        align,
        alignByTranslate,
        height = 0,
        verticalAlign,
        width = 0,
        xx = 0,
        yy = 0
    } = alignOptions || svgWrapper.alignOptions;
    let attribs = {};
    let {
        height: boxHeight = 0,
        width: boxWidth = 0,
        x: boxX = 0,
        y: boxY = 0
    } = box;
    // first call on instanciate
    if (alignOptions) {
        svgWrapper.alignOptions = alignOptions;
    }
    // align
    attribs[(
        alignByTranslate
        ? "translateX"
        : "x"
    )] = Math.round(boxX + xx + (
        align === "center"
        ? 0.5 * (boxWidth - width)
        : align === "right"
        ? 1.0 * (boxWidth - width)
        // default: left align
        : 0
    ));
    // vertical align
    attribs[(
        alignByTranslate
        ? "translateY"
        : "y"
    )] = Math.round(boxY + yy + (
        verticalAlign === "bottom"
        ? 1.0 * (boxHeight - (height))
        : verticalAlign === "middle"
        ? 0.5 * (boxHeight - (height))
        // default: top align
        : 0
    ));
    svgGetset(svgWrapper, attribs);
    return svgWrapper;
}
function svgGetset(wrapper, hash, val) {
// Set or get a given attribute
    let attrSetters = wrapper.attrSetters;
    let {
        element,
        renderer,
        shadows
    } = wrapper;
    let hasSetSymbolSize;
    let key;
    // Android2 requires lower for "text"
    let nodeName = element.nodeName.toLowerCase();
    let ret;
    let skipAttr;
    // single key-value pair
    if (typeof hash === "string" && val !== undefined && val !== null) {
        key = hash;
        hash = {};
        hash[key] = val;
    }
    // used as a getter: first argument is a string, second is undefined
    if (typeof hash === "string") {
        key = hash;
        ret = attr(element, key) || wrapper[key] || 0;
        // "d" is string in animation step
        if (key !== "d" && key !== "visibility") {
            ret = parseFloat(ret);
        }
        return ret;
    }
    // setter
    Object.entries(hash).forEach(function ([
        key, value
    ]) {
        let doTransform;
        skipAttr = false; // reset
        // check for a specific attribute setter
        if (
            attrSetters[key]
            && attrSetters[key].call(wrapper, value, key) !== undefined
        ) {
            return;
        }
        // paths
        switch (key) {
        case "d":
            if (value && value.join) { // join path
                value = value.join(" ");
            }
            break;
        // apply gradients
        case "fill":
            value = renderer.color(value, element, key);
            break;
        //!! // special
        //!! case "isTracker":
            //!! wrapper[key] = value;
            //!! break;
        // rectangle border radius
        case "r":
            if (nodeName === "rect") {
                attr(element, {
                    rx: value,
                    ry: value
                });
                skipAttr = true;
            }
            break;
        // translation and text rotation
        case "translateX":
        case "translateY":
            doTransform = true;
            skipAttr = true;
            break;
        // IE9/MooTools combo: MooTools returns objects instead of numbers
        // and IE9 Beta 2 is unable to cast them. Test again with final IE9.
        case "width":
            value = parseInt(value);
            break;
        // update child tspans x values
        case "x":
            if (nodeName === "text") {
                element.childNodes.forEach(function (child) {
                    // if the x values are equal,
                    // the tspan represents a linebreak
                    if (attr(child, "x") === attr(element, "x")) {
                        //child.setAttribute("x", value);
                        attr(child, "x", value);
                    }
                });
            }
            if (wrapper.rotation) {
                attr(
                    element,
                    "transform",
                    "rotate(" + wrapper.rotation + " " + value + " " +
                    parseInt(hash.y || attr(element, "y")) + ")"
                );
            }
            break;
        }
        // symbols
        if (
            wrapper.symbolName
            && (
                /^(?:x|y|width|height|r|start|end|innerR)/
            ).test(key)
        ) {
            // If one of the symbol size affecting parameters are changed,
            // check all the others only once for each call to an element's
            // .attr() method
            if (!hasSetSymbolSize) {
                [
                    "end",
                    "height",
                    "innerR",
                    "r",
                    "start",
                    "width",
                    "x",
                    "y"
                ].forEach(function (key) {
                    wrapper[key] = hash[key] ?? wrapper[key];
                });
                svgGetset(wrapper, {
                    d: wrapper.renderer.symbols[wrapper.symbolName](
                        wrapper.x,
                        wrapper.y,
                        wrapper.width,
                        wrapper.height,
                        wrapper
                    )
                });
                hasSetSymbolSize = true;
            }
            skipAttr = true;
        }
        // let the shadow follow the main element
        if (
            shadows
            && (
                /^(?:width|height|visibility|x|y|d|transform)$/
            ).test(key)
        ) {
            shadows.forEach(function (shadow) {
                attr(shadow, key, (
                    key === "height"
                    ? Math.max(value - (shadow.cutHeight || 0), 0)
                    : value
                ));
            });
        }
        // Record for animation and quick access without polling the DOM
        wrapper[key] = value;
        // Update transform
        if (doTransform) {
            // apply translate
            if (wrapper.translateX || wrapper.translateY) {
                wrapper.element.setAttribute("transform", (
                    "translate("
                    + (wrapper.translateX || 0)
                    + ","
                    + (wrapper.translateY || 0)
                    + ")"
                ));
            }
        }
        if (key === "text") {
            // Delete bBox memo when the text changes
            if (value !== wrapper.textStr) {
                delete wrapper.bBox;
            }
            wrapper.textStr = value;
            if (wrapper.added) {
                renderer.buildText(wrapper);
            }
        } else if (!skipAttr) {
            attr(element, key, value);
        }
    });
    return wrapper;
}
function uichartSeriesHideOrShow({
    series
}) {
// Set the visibility of the graph
    let display = (
        series.visible
        ? "block"
        : "none"
    );
    // show or hide series
    series.group.element.setAttribute("display", display);
    series.markerGroup.element.setAttribute("display", display);
    // show or hide trackers
    series.tracker.element.setAttribute("display", display);
}
function uichartTooltipHide(chart) {
    let tooltip = chart.tooltip;
    chart.hoverSeries = null;
    tooltip.element.setAttribute("visibility", "hidden");
    tooltip.crosshairs.forEach(function (crosshair) {
        crosshair.element.setAttribute("visibility", "hidden");
    });
}
function uichartTooltipMove(tooltip, xx, yy) {
// Provide a soft movement for the tooltip
    let {
        now
    } = tooltip;
    // get intermediate values for animation
    Object.assign(now, {
        x: (
            (2 * now.x + xx) / 3
        ),
        y: (
            (now.y + yy) / 2
        )
    });
    // move to the intermediate value
    svgGetset(tooltip, now);
    // run on next tick of the mouse tracker
    if (Math.abs(xx - now.x) > 1 || Math.abs(yy - now.y) > 1) {
        // never allow two timeouts
        clearTimeout(tooltip.tooltipTimeout);
        // set the fixed interval ticking for the smooth tooltip
        tooltip.tooltipTimeout = setTimeout(function () {
            // The interval function may still be running during destroy,
            // so check that the chart is really there before calling.
            uichartTooltipMove(tooltip, xx, yy);
        }, 32);
    }
}
function uichartXY(chart, evt) {
// this function will get mouse position chartX, chartY relative to svg-chart
    let rect = chart.elemPlot.getBoundingClientRect();
    return [
        Math.round(
            evt.pageX
            - rect.left
            - window.scrollX
            + document.documentElement.clientLeft
        ),
        Math.round(
            evt.pageY
            - rect.top
            - window.scrollY
            + document.documentElement.clientTop
        )
    ];
}
function uiRenderChart(options) {
    let chart = {};
    let {
        renderTo,
        seriesList,
        title,
        xAxis,
        yAxis
    } = options;
    let renderer = new SVGRenderer();
    // Handle regular options
    chart.options = options;
    chart.axisList = [];
    chart.seriesList = [];
    // Set up auto resize
    chart.xAxis = [];
    chart.yAxis = [];
    // Expose methods and variables
    chart.animation = true;
    chart.counters = {
        color: 0,
        symbol: 0
    };
    // Prepare for first rendering after all data are loaded
    // Get the containing element, determine the size and create the inner
    // container div to hold the chart
    // Create the container
    chart.renderTo = renderTo;
    // remove previous chart
    chart.renderTo.innerHTML = "";
    // get the width
    chart.containerWidth = chart.renderTo.clientWidth;
    chart.chartWidth = Math.max(256, chart.containerWidth);
    // create the inner container
    chart.container = domDivCreate(`
<div
    class="uichartDiv"
    id="${PREFIX + DOM_ID_COUNTER}"
    style="
    height: ${UI_CHART_HEIGHT}px;
    padding-left: ${UI_CHART_LEGEND_WIDTH}px;
    width: ${chart.chartWidth}px;
    "
>
<div
    class="uichartNav"
    style="
    height: ${UI_CHART_HEIGHT}px;
    margin-left: -${UI_CHART_LEGEND_WIDTH}px;
    position: absolute;
    width: ${UI_CHART_LEGEND_WIDTH}px;
    "
>
    <button
        class="uichartAction"
        data-action="zoomReset"
    >reset zoom</button>
    <button
        class="uichartAction"
        data-action="seriesHideAll"
    >hide all</button>
    <button
        class="uichartAction"
        data-action="seriesShowAll"
    >show all</button>
    <div class="uichartLegend" style="height: ${UI_CHART_HEIGHT - 64}px;"></div>
</div>
<svg class="uichartSvg" version="1.1" xmlns="http://www.w3.org/2000/svg">
    <defs>
        <clipPath class="uichartCliprect">
            <rect fill="none" height="0" width="0" x="0" y="0"></rect>
        </clipPath>
    </defs>
    <g class="uichartGrid0" zIndex="1"></g>
    <g class="uichartGrid1" zIndex="1"></g>
    <g class="uichartAxis0" zIndex="2"></g>
    <g class="uichartAxis1" zIndex="2"></g>
    <g class="uichartSeriesList" style="zIndex: 3;"></g>
    <path
        class="uichartCrosshair0"
        fill="none"
        stroke-width="1"
        stroke="#C0C0C0"
        zIndex="2"
        visibility="hidden"
    >
    </path>
    <path
        class="uichartCrosshair1"
        fill="none"
        stroke-width="1"
        stroke="#C0C0C0"
        zIndex="2"
        visibility="hidden"
    >
    </path>
    <g class="uichartAxisLabel0" zIndex="7"></g>
    <g class="uichartAxisLabel1" zIndex="7"></g>
    <!-- class="uichartTooltip" start -->
    <g
        class="uichartTooltip"
        style="padding:0;white-space:nowrap;"
        visibility="hidden"
        zIndex="8"
    >
    <rect
        class="uichartTooltipBorder"
        fill-opacity="0.85"
        fill="#fff"
        height="41"
        rx="5"
        ry="5"
        stroke-width="2"
        x="0"
        y="0"
    >
    </rect>
    <text
        class="uichartTooltipText"
        x="5"
        y="18"
        zIndex="1"
    >
    <tspan
        style="font-size:10px"
        x="5"
    >2010
    </tspan>
    <tspan
        dy="15"
        x="5"
    >Other
    </tspan>
    <tspan
        dx="3"
    >:

    </tspan>
    <tspan
        dx="3"
        style="font-weight:bold"
    >12,908
    </tspan>
    </text>
    </g>
    <!-- class="uichartTooltip" end -->
    <g class="uichartTrackerList" zIndex="9">/g>
</svg>
</div>
    `).firstElementChild;
    DOM_ID_COUNTER += 1;
    chart.renderTo.appendChild(chart.container);
    chart.renderer = renderer;
    renderer.boxWrapper = svgCreateElement(renderer, "svg");
    renderer.boxWrapper.element = chart.container.children[1];
    renderer.boxWrapper.added = true;
    chart.elemPlot = chart.container.children[1];
    // Define a clipping rectangle
    // Draw and return a rectangle
    chart.elemClip = svgCreateElement(renderer, "rect");
    chart.elemClip.id = PREFIX + DOM_ID_COUNTER;
    DOM_ID_COUNTER += 1;
    chart.elemPlot.querySelector(
        ".uichartCliprect"
    ).id = chart.elemClip.id;
    chart.elemClip.element = chart.elemPlot.querySelector(
        ".uichartCliprect rect"
    );
    // The series
    chart.elemSeriesList = renderer.g("series-group");
    chart.elemSeriesList.element = chart.elemPlot.querySelector(
        ".uichartSeriesList"
    );
    // Add the chart to the global lookup
    DBCHART_DICT.set(chart.container.id, chart);
    // object properties
    renderer.box = renderer.boxWrapper.element;
    // Page url used for internal references. #24, #672, #1070
    // escape parantheses and quotes
    renderer.url = location.href.split("#")[0].replace((
        /([(")])/g
    ), "\\$1");
    renderer.gradients = {}; // Object where gradient SvgElements are stored
    chartGetMargins(chart);
    // get axes
    // Create the Axis instances based on the config options
    [
        xAxis, yAxis
    ].forEach(function (userOptions, ii) {
        // Create a new axis object
        let axis = new Axis();
        let isXAxis = !ii;
        // Initialize the axis
        // Flag, isXAxis
        axis.isXAxis = isXAxis;
        axis.xOrY = (
            isXAxis
            ? "x"
            : "y"
        );
        axis.side = (
            axis.isXAxis
            // bottom
            ? 2
            // left
            : 3
        );
        // Merge and set options
        axis.options = Object.assign(
            {
                categoryList: [],
                lineColor: "#C0D0E0",
                lineWidth: 1,
                maxPadding: 0.01,
                minPadding: 0.01,
                startOnTick: false,
                tickPixelInterval: 100,
                tickPosition: "outside",
                tickWidth: 1,
                type: "linear" // linear, logarithmic or datetime
            },
            (
                axis.isXAxis
                ? {}
                // This options set extends the defaultOptions for Y axes
                : {
                    endOnTick: true,
                    lineWidth: 0,
                    maxPadding: 0.05,
                    minPadding: 0.05,
                    showLastLabel: true,
                    startOnTick: true,
                    tickPixelInterval: 72,
                    tickWidth: 0
                }
            ),
            userOptions
        );
        // Flag, stagger lines or not
        axis.minPixelPadding = 0;
        axis.chart = chart;
        // Initial categoryList
        axis.categoryList = axis.options.categoryList;
        // Elements
        // Linked axis.
        // Tick intervals
        axis.tickmarkOffset = (
            axis.options.categoryList.length
            ? 0.5
            : 0
        );
        // Major ticks
        axis.tickDict = {};
        // Axis metrics
        axis.len = 0;
        axis.userMinRange = axis.options.minRange || axis.options.maxZoom;
        axis.minRange = axis.userMinRange;
        axis.range = axis.options.range;
        // Min and max in the data
        // The axis range
        axis.max = null;
        axis.min = null;
        // User set min and max
        // Run Axis
        // Register
        chart.axisList.push(axis);
        chart[(
            isXAxis
            ? "xAxis"
            : "yAxis"
        )].push(axis);
        axis.series = []; // populated by Series
    });
    // Initialize the series, called internally before render time
    seriesList.forEach(function ({
        data,
        name
    }) {
        let series = new LineSeries();
        // Register it in the chart
        chart.seriesList.push(series);
        series.chart = chart;
        // merge with plotOptions
        series.options = Object.assign({
            animation: {
                duration: 1000
            },
            cropThreshold: 300,
            events: {},
            marker: {
                enabled: true,
                lineColor: "#FFFFFF",
                lineWidth: 0,
                radius: 4,
                states: {
                    hover: {
                        enabled: true
                    },
                    select: {
                        fillColor: "#FFFFFF",
                        lineColor: "#000000",
                        lineWidth: 2
                    }
                }
            },
            point: {
                events: {}
            },
            pointRange: 0,
            shadow: true,
            states: {
                hover: {
                    marker: {}
                },
                select: {
                    marker: {}
                }
            }
        }, {
            data,
            name
        });
        // bind the axes
        // Set the xAxis and yAxis properties of cartesian series,
        // and register the series
        // repeat for xAxis and yAxis
        ["xAxis", "yAxis"].forEach(function (axisName) {
            // loop through the chart's axis objects
            chart[axisName].forEach(function (axis) {
                // apply if the series xAxis or yAxis option Mathches the
                // number of the axis, or if undefined, use the first axis
                if (
                    (series.options[axisName] === axis.options.index)
                    || (
                        series.options[axisName] === undefined
                        && axis.options.index === 0
                    )
                ) {
                    // register this series in the axis.series lookup
                    axis.series.push(series);
                    // set this series.xAxis or series.yAxis reference
                    series[axisName] = axis;
                }
            });
        });
        // set some variables
        Object.assign(series, {
            name: series.options.name,
            state: "",
            pointAttr: {},
            visible: true
        });
        // Get the series' color
        let defaultColors = [
            "#4572A7",
            "#AA4643",
            "#89A54E",
            "#80699B",
            "#3D96AE",
            "#DB843D",
            "#92A8CD",
            "#A47D7C",
            "#B5CA92"
        ];
        series.color = defaultColors[chart.counters.color];
        chart.counters.color = (
            (chart.counters.color + 1)
            % defaultColors.length
        );
        // Get the series' symbol
        let defaultSymbols = [
            "circle",
            "diamond",
            "square",
            "triangle",
            "triangle-down"
        ];
        series.symbol = defaultSymbols[chart.counters.symbol];
        chart.counters.symbol = (
            (chart.counters.symbol + 1)
            % defaultSymbols.length
        );
        // set the data
        // reset properties
        series.xIncrement = null;
        series.pointRange = (
            series.xAxis.categoryList.length
            ? 1
            : series.options.pointRange
        );
        // parallel arrays
        let xData = [];
        let yData = [];
        // In turbo mode, only one- or twodimensional arrays of numbers are
        // allowed. The first value is tested, and we assume that all the rest
        // are defined the same way. Although the "for" loops are similar, they
        // are repeated inside each if-else conditional for max performance.
        // assume all points are numbers
        let xx = 0;
        data.forEach(function (val, ii) {
            xData[ii] = xx;
            yData[ii] = val;
            xx += 1;
        });
        series.xIncrement = xx;
        // Unsorted data is not supported by the line tooltip as well as data
        // grouping and navigation in Stock charts (#725)
        // if (xData.length > 1 && xData[1] < xData[0]) {
        //     error(15);
        // }
        series.options.data = data;
        series.xData = xData;
        series.yData = yData;
        // reset minRange (#878)
        series.xAxis.minRange = series.xAxis.userMinRange;
    });
    //
    // Run MouseTracker
    // The tooltip object
    let tooltip;
    let tooltipBorder;
    let tooltipText;
    tooltip = renderer.g("tooltip");
    tooltip.element = chart.elemPlot.querySelector(".uichartTooltip");
    tooltip.added = true;
    // create the border box if it is not already present
    tooltipBorder = svgCreateElement(renderer, "rect");
    tooltipBorder.element = tooltip.element.querySelector(
        ".uichartTooltipBorder"
    );
    tooltipBorder.added = true;
    // create the label
    // Add a label, a text item that can hold a colored or gradient background
    // as well as a border and shadow.
    tooltipText = svgCreateElement(renderer, "rect");
    tooltipText.element = tooltip.element.querySelector(".uichartTooltipText");
    tooltipText.added = true;
    //
    tooltip.chart = chart;
    // Keep track of the current series
    // List of crosshairs
    tooltip.crosshairs = Array.from(new Array(2)).map(function (ignore, ii) {
        let crosshair = chart.renderer.path();
        crosshair.element = chart.elemPlot.querySelector(
            `.uichartCrosshair${ii}`
        );
        return crosshair;
    });
    // Current values of x and y when animating
    tooltip.now = {x: 0, y: 0};
    // create the label
    // Add a label, a text item that can hold a colored or gradient background
    // as well as a border and shadow.
    let bBox;
    let deferredAttr = {};
    let baselineOffset;
    let attrSetters = tooltip.attrSetters;
    /**
     * This function runs after the tooltip is added to the DOM
     * (when the bounding box is available), and after the text of the
     * tooltip is updated to detect the new bounding
     * box and reflect it in the border box.
     */
    function updateBoxSize() {
        let style = tooltipText.element.style;
        bBox = tooltipText.getBBox();
        tooltip.width = (bBox.width || 0) + 10;
        tooltip.height = (bBox.height || 0) + 10;
        // update the tooltip-scoped y offset
        baselineOffset = 5 + renderer.fontMetrics(style && style.fontSize).b;
        // apply the tooltipBorder attributes
        svgGetset(tooltipBorder, Object.assign({
            width: tooltip.width,
            height: tooltip.height
        }, deferredAttr));
        deferredAttr = null;
    }
    /**
     * This function runs after setting text or padding, but only if
     * padding is changed
     */
    function updateTextPadding() {
        let x2 = 5;
        let y2;
        // determin y2 based on the baseline
        y2 = baselineOffset;
        // update if anything changed
        if (x2 !== tooltipText.x2 || y2 !== tooltipText.y2) {
            svgGetset(tooltipText, {
                x: x2,
                y: y2
            });
        }
        // record current values
        tooltipText.x = x2;
        tooltipText.y = y2;
    }
    /**
     * Set a box attribute, or defer it if the box is not yet created
     * @param {Object} key
     * @param {Object} value
     */
    function boxAttr(key, value) {
        svgGetset(tooltipBorder, key, value);
    }
    /*
     * Add specific attribute setters.
     */
    // apply these to the box but not to the text
    attrSetters["stroke-width"] = function (value, key) {
        boxAttr(key, value);
        return false;
    };
    attrSetters.stroke = attrSetters.fill = attrSetters.r = function (value, key) { //jslint-ignore-line
        boxAttr(key, value);
        return false;
    };
    // rename attributes
    attrSetters.x = function (value) {
        tooltip.x = value; // for animation getter
        svgGetset(
            tooltip,
            "translateX",
            Math.round(value)
        );
        return false;
    };
    attrSetters.y = function (value) {
        tooltip.y = value;
        svgGetset(tooltip, "translateY", Math.round(value));
        return false;
    };
    // Redirect certain methods to either the box or the text
    //
    tooltip.element.setAttribute("visibility", "hidden");
    /**
     * After the text element is added, get the desired size of the border
     * box
     * and add it before the text in the DOM.
     */
    chart.tooltip = tooltip;
    //
    // Set the JS events on the container element
    // The mousemove, touchmove and touchstart event handler
    chart.container.onmousemove = function (evt) {
        let chartX;
        let chartY;
        let point;
        let series;
        // normalize mouse position
        [
            chartX, chartY
        ] = uichartXY(chart, evt);
        point = chart.hoverSeries?.tooltipPoints[chartX - chart.plotLeft];
        // Show the tooltip and run mouse over events (#977)
        // a new point is hovered, refresh the tooltip
        if (
            !chart.hoverSeries
            || point === chart.hoverPoint
            || !chartIsInsidePlot(
                chart,
                chartX - chart.plotLeft,
                chartY - chart.plotTop
            )
        ) {
            return;
        }
        series = point.series;
        // hover this
        chart.hoverPoint = point;
        // Refresh the tooltip's text and position.
        let placedTooltipPoint;
        // get the reference point coordinates
        //
        let plotX = Math.round(point.plotX);
        let plotY = Math.round(point.plotY + series.yAxis.top - chart.plotTop);
        // single point tooltip
        // Return the configuration hash needed for the data label
        // and tooltip formatters
        // update the inner HTML
        // show it
        tooltip.element.setAttribute("visibility", "visible");
        // update text
        // apply these to the box and the text alike
        svgGetset(tooltipText, "text", (
            `<tspan style="font-size: 10px">`
            + `${point.name || point.category}</tspan><br/>`
            + `<tspan style="color: ${series.color};">${series.name}</tspan>`
            + `:<b>${point.y}</b><br/>`
        ));
        updateBoxSize();
        updateTextPadding();
        // set the stroke color of the box
        svgGetset(tooltip, {
            stroke: series.color
        });
        // Set up the variables
        placedTooltipPoint = {};
        let plotLeft = chart.plotLeft;
        let plotTop = chart.plotTop;
        let plotWidth = chart.plotWidth;
        let plotHeight = chart.plotHeight;
        placedTooltipPoint.x = plotX + plotLeft - tooltip.width - 12;
        // 15 means the point is 15 pixels up from the bottom of the tooltip
        placedTooltipPoint.y = plotY - tooltip.height + plotTop + 15;
        let alignedRight;
        // It is too far to the left, adjust it
        if (placedTooltipPoint.x < 7) {
            placedTooltipPoint.x = plotLeft + Math.max(plotX, 0) + 12;
        }
        // Test to see if the tooltip is too far to the right, if it is, move
        // it back to be inside and then up to not cover the point.
        if ((placedTooltipPoint.x + tooltip.width) > (plotLeft + plotWidth)) {
            placedTooltipPoint.x -= (
                (placedTooltipPoint.x + tooltip.width) - (plotLeft + plotWidth)
            );
            placedTooltipPoint.y = plotY - tooltip.height + plotTop - 12;
            alignedRight = true;
        }
        // If it is now above the plot area, align it to the top of the plot
        // area
        if (placedTooltipPoint.y < plotTop + 5) {
            placedTooltipPoint.y = plotTop + 5;
            // If the tooltip is still covering the point, move it below instead
            if (
                alignedRight && plotY >= placedTooltipPoint.y
                && plotY <= (placedTooltipPoint.y + tooltip.height)
            ) {
                placedTooltipPoint.y = plotY + plotTop + 12; // below
            }
        }
        // Now if the tooltip is below the chart, move it up. It's better to
        // cover the point than to disappear outside the chart. #834.
        if (placedTooltipPoint.y + tooltip.height > plotTop + plotHeight) {
            // below
            placedTooltipPoint.y = Math.max(
                plotTop,
                plotTop + plotHeight - tooltip.height - 12
            );
        }
        //
        // do the move
        uichartTooltipMove(
            tooltip,
            Math.round(placedTooltipPoint.x),
            Math.round(placedTooltipPoint.y),
            plotX + chart.plotLeft,
            plotY + chart.plotTop
        );
        // crosshairs
        tooltip.crosshairs.forEach(function (ignore, ii) {
            svgGetset(tooltip.crosshairs[ii], {
                d: series[(
                    ii
                    ? "yAxis"
                    : "xAxis"
                )].getPlotLinePath(
                    (
                        ii
                        ? pick(point.stackY, point.y)
                        : point.x
                    ), // #814
                    1
                ),
                visibility: "visible"
            });
        });
    };
    //
    // render the legend
    chart.container.querySelector(
        ".uichartLegend"
    ).innerHTML = chart.seriesList.map(function (series, ii) {
        return (`
<a
    class="uichartAction uichartLegendElem"
    data-action="seriesVisibilityToggle"
    data-series-ii="${ii}"
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
    d="${renderer.symbols[series.symbol](4, 4, 8, 8).join(" ")}"
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
    // Title
    // add title and subtitle
    // Show the title and subtitle of the chart
    chart.elemTitle = renderer.text(title, 0, 0);
    chart.elemTitle.element.setAttribute("class", "uichartTitle");
    chart.elemTitle.element.setAttribute("text-anchor", "middle");
    chart.elemTitle.element.setAttribute("zIndex", "4");
    chart.elemTitle.add();
    svgAlign({
        alignOptions: {
            align: "center",
            style: {
                color: "#3E576F",
                fontSize: "16px"
            },
            y: 15
        },
        box: chart.spacingBox,
        svgWrapper: chart.elemTitle
    });
    chartGetMargins(chart);
    // Set flag
    chart.hasRendered = true;
    uichartOnAction({
        type: "resize"
    });
    // init event-handling
    chart.elemPlot.onwheel = function (evt) {
        evt.preventDefault();
        evt.stopPropagation();
        uichartOnAction(evt);
    };
    // When the mouse leaves the container, hide the tracking (tooltip).
    chart.elemPlot.onmouseleave = function () {
        uichartTooltipHide(chart);
    };
}
function uiAnimate(elem, prop) {
    function uiAnimateUpdate(fx) {
    // Simple function for setting a style value
        let ends;
        if (fx.prop !== "d") {
            svgGetset(elem, fx.prop, fx.now);
            return;
        }
        // animate paths
        // Normally start and end should be set in state == 0, but sometimes,
        // for reasons unknown, this doesn't happen. Perhaps state == 0 is
        // skipped in these cases
        if (!fx.started) {
            // Prepare start and end values so that the path can be animated
            // one to one
            ends = [elem.d.split(" "), elem.toD];
            fx.start = ends[0];
            fx.end = ends[1];
            fx.started = true;
        }
        // interpolate each value of the path
        // Interpolate each value of the path and return the array
        // land on the final path without adjustment points appended in the ends
        svgGetset(elem, "d", (
            fx.pos === 1
            ? elem.toD
            : fx.start.map(function (val, ii) {
                let num = parseFloat(val);
                return (
                    !Number.isFinite(num)
                    // a letter instruction like M or L
                    ? val
                    : num + fx.pos * (parseFloat(fx.end[ii] - num))
                );
            })
        ));
    }
    Object.entries(prop).forEach(function ([
        key, val
    ]) {
        let fx = {};
        fx.elem = elem;
        fx.prop = key;
        let fxFrom = svgGetset(fx.elem, fx.prop);
        // Start an animation from one number to another
        fx.end = parseFloat(val);
        fx.now = fxFrom;
        fx.pos = 0;
        fx.start = fxFrom;
        fx.startTime = Date.now();
        fx.state = 0;
        function timer() {
            // Each step of an animation
            let nn;
            let tt = Date.now();
            if (tt < 400 + fx.startTime) {
                nn = tt - fx.startTime;
                fx.state = nn / 400;
                // Perform the easing function, defaults to swing
                fx.pos = -0.5 * Math.cos(fx.state * Math.PI) + 0.5;
                fx.now = fx.start + ((fx.end - fx.start) * fx.pos);
                // Perform the next step of the animation
                uiAnimateUpdate(fx);
                return true;
            }
            fx.now = fx.end;
            fx.pos = 1;
            fx.state = 1;
            uiAnimateUpdate(fx);
            prop[fx.prop] = true;
        }
        timer.elem = fx.elem;
        if (
            timer() && UI_ANIMATE_TIMERLIST.push(timer) && !UI_ANIMATE_TIMERID
        ) {
            UI_ANIMATE_TIMERID = setInterval(function () {
                UI_ANIMATE_TIMERLIST = UI_ANIMATE_TIMERLIST.filter(function (
                    timer
                ) {
                    return timer();
                });
                if (!UI_ANIMATE_TIMERLIST.length) {
                    clearInterval(UI_ANIMATE_TIMERID);
                    UI_ANIMATE_TIMERID = undefined;
                }
            }, 13);
        }
    });
}
function uiAnimateStop(elem) {
    // go in reverse order so anything added to the queue during the
    // loop is ignored
    let ii = UI_ANIMATE_TIMERLIST.length;
    while (ii > 0) {
        ii -= 1;
        if (UI_ANIMATE_TIMERLIST[ii].elem === elem) {
            UI_ANIMATE_TIMERLIST.splice(ii, 1);
        }
    }
}
window.addEventListener("resize", uichartOnAction);
document.body.addEventListener("click", uichartOnAction);
