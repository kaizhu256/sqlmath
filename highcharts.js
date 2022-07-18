/*jslint browser, unordered*/
/*global
    css
    defined
    fireEvent
    mathMax
    mathRound
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
let UI_CHART_MARGIN_BOTTOM = 32;
let UI_CHART_MARGIN_RIGHT = 10;
let UI_CHART_PLOT_TOP = 10;
/*jslint-disable*/
(function () {
    "use strict";
let $ = undefined;
// encapsulated variables
let UNDEFINED,
    doc = document,
    win = window,
    math = Math,
    mathRound = math.round,
    mathFloor = math.floor,
    mathCeil = math.ceil,
    mathMax = math.max,
    mathMin = math.min,
    mathAbs = math.abs,
    mathCos = math.cos,
    mathSin = math.sin,
    mathPI = math.PI,
    deg2rad = mathPI * 2 / 360,
    // some variables
    userAgent = navigator.userAgent,
    isOpera = win.opera,
    isIE = /msie/i.test(userAgent) && !isOpera,
    docMode8 = doc.documentMode === 8,
    isWebKit = /AppleWebKit/.test(userAgent),
    isFirefox = /Firefox/.test(userAgent),
    isTouchDevice = /(Mobile|Android|Windows Phone)/.test(userAgent),
    SVG_NS = 'http://www.w3.org/2000/svg',
    hasSVG = !!doc.createElementNS && !!doc.createElementNS(SVG_NS, 'svg').createSVGRect,
    hasBidiBug = isFirefox && parseInt(userAgent.split('Firefox/')[1], 10) < 4, // issue #38
    hasTouch = doc.documentElement.ontouchstart !== UNDEFINED,
    symbolSizes = {},
    idCounter = 0,
    garbageBin,
    defaultOptions,
    dateFormat, // function
    pathAnim,
    timeUnits,
    noop = function () {},
    charts = [],
    // some constants for frequently used strings
    DIV = 'div',
    ABSOLUTE = 'absolute',
    RELATIVE = 'relative',
    HIDDEN = 'hidden',
    PREFIX = 'highcharts-',
    VISIBLE = 'visible',
    NONE = 'none',
    M = 'M',
    L = 'L',
    /*
     * Empirical lowest possible opacities for TRACKER_FILL
     * IE6: 0.002
     * IE7: 0.002
     * IE8: 0.002
     * IE9: 0.00000000001 (unlimited)
     * IE10: 0.0001 (exporting only)
     * FF: 0.00000000001 (unlimited)
     * Chrome: 0.000001
     * Safari: 0.000001
     * Opera: 0.00000000001 (unlimited)
     */
    TRACKER_FILL = 'rgba(192,192,192,' + (hasSVG ? 0.0001 : 0.002) + ')', // invisible but clickable
    //TRACKER_FILL = 'rgba(192,192,192,0.5)',
    NORMAL_STATE = '',
    HOVER_STATE = 'hover',
    SELECT_STATE = 'select',
    MILLISECOND = 'millisecond',
    SECOND = 'second',
    MINUTE = 'minute',
    HOUR = 'hour',
    DAY = 'day',
    WEEK = 'week',
    MONTH = 'month',
    YEAR = 'year',
    // constants for attributes
    FILL = 'fill',
    STOPS = 'stops',
    STROKE = 'stroke',
    STROKE_WIDTH = 'stroke-width',
    // time methods, changed based on whether or not UTC is used
    makeTime,
    getMinutes,
    getHours,
    getDay,
    getDate,
    getMonth,
    getFullYear,
    setMinutes,
    setHours,
    setDate,
    setMonth,
    setFullYear,
    // lookup over the types and the associated classes
    seriesTypes = {};
// The Highcharts namespace
win.Highcharts = {};
/**
 * Extend an object with the members of another
 * @param {Object} a The object to be extended
 * @param {Object} b The object to add to the first one
 */
function extend(a, b) {
    let n;
    if (!a) {
        a = {};
    }
    for (n in b) {
        a[n] = b[n];
    }
    return a;
}
/**
 * Take an array and turn into a hash with even number arguments as keys and odd numbers as
 * values. Allows creating constants for commonly used style properties, attributes etc.
 * Avoid it in performance critical situations like looping
 */
function hash() {
    let i = 0,
        args = arguments,
        length = args.length,
        obj = {};
    for (; i < length; i++) {
        obj[args[i++]] = args[i];
    }
    return obj;
}
/**
 * Shortcut for parseInt
 * @param {Object} s
 * @param {Number} mag Magnitude
 */
function pInt(s, mag) {
    return parseInt(s, mag || 10);
}
/**
 * Check for string
 * @param {Object} s
 */
function isString(s) {
    return typeof s === 'string';
}
/**
 * Check for object
 * @param {Object} obj
 */
function isObject(obj) {
    return typeof obj === 'object';
}
/**
 * Check for array
 * @param {Object} obj
 */
function isArray(obj) {
    return Object.prototype.toString.call(obj) === '[object Array]';
}
/**
 * Check for number
 * @param {Object} n
 */
function isNumber(n) {
    return typeof n === 'number';
}
function log2lin(num) {
    return math.log(num) / math.LN10;
}
function lin2log(num) {
    return math.pow(10, num);
}
/**
 * Remove last occurence of an item from an array
 * @param {Array} arr
 * @param {Mixed} item
 */
function erase(arr, item) {
    let i = arr.length;
    while (i--) {
        if (arr[i] === item) {
            arr.splice(i, 1);
            break;
        }
    }
    //return arr;
}
/**
 * Returns true if the object is not null or undefined. Like MooTools' jQuery.defined.
 * @param {Object} obj
 */
function defined(obj) {
    return obj !== UNDEFINED && obj !== null;
}
/**
 * Set or get an attribute or an object of attributes. Can't use jQuery attr because
 * it attempts to set expando properties on the SVG element, which is not allowed.
 *
 * @param {Object} elem The DOM element to receive the attribute(s)
 * @param {String|Object} prop The property or an abject of key-value pairs
 * @param {String} value The value if a single property is set
 */
function attr(elem, prop, value) {
    let key,
        setAttribute = 'setAttribute',
        ret;
    // if the prop is a string
    if (isString(prop)) {
        // set the value
        if (defined(value)) {
            elem[setAttribute](prop, value);
        // get the value
        } else if (elem && elem.getAttribute) { // elem not defined when printing pie demo...
            ret = elem.getAttribute(prop);
        }
    // else if prop is defined, it is a hash of key/value pairs
    } else if (defined(prop) && isObject(prop)) {
        for (key in prop) {
            elem[setAttribute](key, prop[key]);
        }
    }
    return ret;
}
/**
 * Check if an element is an array, and if not, make it into an array. Like
 * MooTools' jQuery.splat.
 */
function splat(obj) {
    return isArray(obj) ? obj : [obj];
}
/**
 * Return the first value that is defined. Like MooTools' jQuery.pick.
 */
function pick() {
    let args = arguments,
        i,
        arg,
        length = args.length;
    for (i = 0; i < length; i++) {
        arg = args[i];
        if (typeof arg !== 'undefined' && arg !== null) {
            return arg;
        }
    }
}
/**
 * Set CSS on a given element
 * @param {Object} el
 * @param {Object} styles Style object with camel case property names
 */
function css(el, styles) {
    if (isIE) {
        if (styles && styles.opacity !== UNDEFINED) {
            styles.filter = 'alpha(opacity=' + (styles.opacity * 100) + ')';
        }
    }
    extend(el.style, styles);
}
/**
 * Utility function to create element with attributes and styles
 * @param {Object} tag
 * @param {Object} attribs
 * @param {Object} styles
 * @param {Object} parent
 * @param {Object} nopad
 */
function createElement(tag, attribs, styles, parent, nopad) {
    let el = doc.createElement(tag);
    if (attribs) {
        extend(el, attribs);
    }
    if (nopad) {
        css(el, {padding: 0, border: NONE, margin: 0});
    }
    if (styles) {
        css(el, styles);
    }
    if (parent) {
        parent.appendChild(el);
    }
    return el;
}
/**
 * Extend a prototyped class by new members
 * @param {Object} parent
 * @param {Object} members
 */
function extendClass(parent, members) {
    let object = function () {};
    object.prototype = new parent();
    extend(object.prototype, members);
    return object;
}
/**
 * How many decimals are there in a number
 */
function getDecimals(number) {
    number = (number || 0).toString();
    return number.indexOf('.') > -1 ?
        number.split('.')[1].length :
        0;
}
/**
 * Format a number and return a string based on input settings
 * @param {Number} number The input number to format
 * @param {Number} decimals The amount of decimals
 * @param {String} decPoint The decimal point, defaults to the one given in the lang options
 * @param {String} thousandsSep The thousands separator, defaults to the one given in the lang options
 */
function numberFormat(number, decimals, decPoint, thousandsSep) {
    let lang = defaultOptions.lang,
        // http://kevin.vanzonneveld.net/techblog/article/javascript_equivalent_for_phps_number_format/
        n = number,
        c = decimals === -1 ?
            getDecimals(number) :
            (isNaN(decimals = mathAbs(decimals)) ? 2 : decimals),
        d = decPoint === undefined ? lang.decimalPoint : decPoint,
        t = thousandsSep === undefined ? lang.thousandsSep : thousandsSep,
        s = n < 0 ? "-" : "",
        i = String(pInt(n = mathAbs(+n || 0).toFixed(c))),
        j = i.length > 3 ? i.length % 3 : 0;
    return s + (j ? i.substr(0, j) + t : "") + i.substr(j).replace(/(\d{3})(?=\d)/g, "$1" + t) +
        (c ? d + mathAbs(n - i).toFixed(c).slice(2) : "");
}
/**
 * Pad a string to a given length by adding 0 to the beginning
 * @param {Number} number
 * @param {Number} length
 */
function pad(number, length) {
    // Create an array of the remaining length +1 and join it with 0's
    return new Array((length || 2) + 1 - String(number).length).join(0) + number;
}
/**
 * Wrap a method with extended functionality, preserving the original function
 * @param {Object} obj The context object that the method belongs to
 * @param {String} method The name of the method to extend
 * @param {Function} func A wrapper function callback. This function is called with the same arguments
 * as the original function, except that the original function is unshifted and passed as the first
 * argument.
 */
function wrap(obj, method, func) {
    let proceed = obj[method];
    obj[method] = function () {
        let args = Array.prototype.slice.call(arguments);
        args.unshift(proceed);
        return func.apply(this, args);
    };
}
/**
 * Based on http://www.php.net/manual/en/function.strftime.php
 * @param {String} format
 * @param {Number} timestamp
 * @param {Boolean} capitalize
 */
dateFormat = function (format, timestamp, capitalize) {
    if (!defined(timestamp) || isNaN(timestamp)) {
        return 'Invalid date';
    }
    format = pick(format, '%Y-%m-%d %H:%M:%S');
    let date = new Date(timestamp),
        key, // used in for constuct below
        // get the basic time values
        hours = date[getHours](),
        day = date[getDay](),
        dayOfMonth = date[getDate](),
        month = date[getMonth](),
        fullYear = date[getFullYear](),
        lang = defaultOptions.lang,
        langWeekdays = lang.weekdays,
        /* // uncomment this and the 'W' format key below to enable week numbers
        weekNumber = function () {
            let clone = new Date(date.valueOf()),
                day = clone[getDay]() == 0 ? 7 : clone[getDay](),
                dayNumber;
            clone.setDate(clone[getDate]() + 4 - day);
            dayNumber = mathFloor((clone.getTime() - new Date(clone[getFullYear](), 0, 1, -6)) / 86400000);
            return 1 + mathFloor(dayNumber / 7);
        },
        */
        // list all format keys
        replacements = {
            // Day
            'a': langWeekdays[day].substr(0, 3), // Short weekday, like 'Mon'
            'A': langWeekdays[day], // Long weekday, like 'Monday'
            'd': pad(dayOfMonth), // Two digit day of the month, 01 to 31
            'e': dayOfMonth, // Day of the month, 1 through 31
            // Week (none implemented)
            //'W': weekNumber(),
            // Month
            'b': lang.shortMonths[month], // Short month, like 'Jan'
            'B': lang.months[month], // Long month, like 'January'
            'm': pad(month + 1), // Two digit month number, 01 through 12
            // Year
            'y': fullYear.toString().substr(2, 2), // Two digits year, like 09 for 2009
            'Y': fullYear, // Four digits year, like 2009
            // Time
            'H': pad(hours), // Two digits hours in 24h format, 00 through 23
            'I': pad((hours % 12) || 12), // Two digits hours in 12h format, 00 through 11
            'l': (hours % 12) || 12, // Hours in 12h format, 1 through 12
            'M': pad(date[getMinutes]()), // Two digits minutes, 00 through 59
            'p': hours < 12 ? 'AM' : 'PM', // Upper case AM or PM
            'P': hours < 12 ? 'am' : 'pm', // Lower case AM or PM
            'S': pad(date.getSeconds()), // Two digits seconds, 00 through  59
            'L': pad(mathRound(timestamp % 1000), 3) // Milliseconds (naming from Ruby)
        };
    // do the replaces
    for (key in replacements) {
        while (format.indexOf('%' + key) !== -1) { // regex would do it in one line, but this is faster
            format = format.replace('%' + key, replacements[key]);
        }
    }
    // Optionally capitalize the string and return
    return capitalize ? format.substr(0, 1).toUpperCase() + format.substr(1) : format;
};
/**
 * Take an interval and normalize it to multiples of 1, 2, 2.5 and 5
 * @param {Number} interval
 * @param {Array} multiples
 * @param {Number} magnitude
 * @param {Object} options
 */
function normalizeTickInterval(interval, multiples, magnitude, options) {
    let normalized, i;
    // round to a tenfold of 1, 2, 2.5 or 5
    magnitude = pick(magnitude, 1);
    normalized = interval / magnitude;
    // multiples for a linear scale
    if (!multiples) {
        multiples = [1, 2, 2.5, 5, 10];
        // the allowDecimals option
        if (options && options.allowDecimals === false) {
            if (magnitude === 1) {
                multiples = [1, 2, 5, 10];
            } else if (magnitude <= 0.1) {
                multiples = [1 / magnitude];
            }
        }
    }
    // normalize the interval to the nearest multiple
    for (i = 0; i < multiples.length; i++) {
        interval = multiples[i];
        if (normalized <= (multiples[i] + (multiples[i + 1] || multiples[i])) / 2) {
            break;
        }
    }
    // multiply back to the correct magnitude
    interval *= magnitude;
    return interval;
}
/**
 * Get a normalized tick interval for dates. Returns a configuration object with
 * unit range (interval), count and name. Used to prepare data for getTimeTicks.
 * Previously this logic was part of getTimeTicks, but as getTimeTicks now runs
 * of segments in stock charts, the normalizing logic was extracted in order to
 * prevent it for running over again for each segment having the same interval.
 * #662, #697.
 */
function normalizeTimeTickInterval(tickInterval, unitsOption) {
    let units = unitsOption || [[
                MILLISECOND, // unit name
                [1, 2, 5, 10, 20, 25, 50, 100, 200, 500] // allowed multiples
            ], [
                SECOND,
                [1, 2, 5, 10, 15, 30]
            ], [
                MINUTE,
                [1, 2, 5, 10, 15, 30]
            ], [
                HOUR,
                [1, 2, 3, 4, 6, 8, 12]
            ], [
                DAY,
                [1, 2]
            ], [
                WEEK,
                [1, 2]
            ], [
                MONTH,
                [1, 2, 3, 4, 6]
            ], [
                YEAR,
                null
            ]],
        unit = units[units.length - 1], // default unit is years
        interval = timeUnits[unit[0]],
        multiples = unit[1],
        count,
        i;
    // loop through the units to find the one that best fits the tickInterval
    for (i = 0; i < units.length; i++) {
        unit = units[i];
        interval = timeUnits[unit[0]];
        multiples = unit[1];
        if (units[i + 1]) {
            // lessThan is in the middle between the highest multiple and the next unit.
            let lessThan = (interval * multiples[multiples.length - 1] +
                        timeUnits[units[i + 1][0]]) / 2;
            // break and keep the current unit
            if (tickInterval <= lessThan) {
                break;
            }
        }
    }
    // prevent 2.5 years intervals, though 25, 250 etc. are allowed
    if (interval === timeUnits[YEAR] && tickInterval < 5 * interval) {
        multiples = [1, 2, 5];
    }
    // prevent 2.5 years intervals, though 25, 250 etc. are allowed
    if (interval === timeUnits[YEAR] && tickInterval < 5 * interval) {
        multiples = [1, 2, 5];
    }
    // get the count
    count = normalizeTickInterval(tickInterval / interval, multiples);
    return {
        unitRange: interval,
        count: count,
        unitName: unit[0]
    };
}
/**
 * Set the tick positions to a time unit that makes sense, for example
 * on the first of each month or on every Monday. Return an array
 * with the time positions. Used in datetime axes as well as for grouping
 * data on a datetime axis.
 *
 * @param {Object} normalizedInterval The interval in axis values (ms) and the count
 * @param {Number} min The minimum in axis values
 * @param {Number} max The maximum in axis values
 * @param {Number} startOfWeek
 */
function getTimeTicks(normalizedInterval, min, max, startOfWeek) {
    let tickPositions = [],
        i,
        higherRanks = {},
        useUTC = defaultOptions.global.useUTC,
        minYear, // used in months and years as a basis for Date.UTC()
        minDate = new Date(min),
        interval = normalizedInterval.unitRange,
        count = normalizedInterval.count;
    if (defined(min)) { // #1300
        if (interval >= timeUnits[SECOND]) { // second
            minDate.setMilliseconds(0);
            minDate.setSeconds(interval >= timeUnits[MINUTE] ? 0 :
                count * mathFloor(minDate.getSeconds() / count));
        }
        if (interval >= timeUnits[MINUTE]) { // minute
            minDate[setMinutes](interval >= timeUnits[HOUR] ? 0 :
                count * mathFloor(minDate[getMinutes]() / count));
        }
        if (interval >= timeUnits[HOUR]) { // hour
            minDate[setHours](interval >= timeUnits[DAY] ? 0 :
                count * mathFloor(minDate[getHours]() / count));
        }
        if (interval >= timeUnits[DAY]) { // day
            minDate[setDate](interval >= timeUnits[MONTH] ? 1 :
                count * mathFloor(minDate[getDate]() / count));
        }
        if (interval >= timeUnits[MONTH]) { // month
            minDate[setMonth](interval >= timeUnits[YEAR] ? 0 :
                count * mathFloor(minDate[getMonth]() / count));
            minYear = minDate[getFullYear]();
        }
        if (interval >= timeUnits[YEAR]) { // year
            minYear -= minYear % count;
            minDate[setFullYear](minYear);
        }
        // week is a special case that runs outside the hierarchy
        if (interval === timeUnits[WEEK]) {
            // get start of current week, independent of count
            minDate[setDate](minDate[getDate]() - minDate[getDay]() +
                pick(startOfWeek, 1));
        }
        // get tick positions
        i = 1;
        minYear = minDate[getFullYear]();
        let time = minDate.getTime(),
            minMonth = minDate[getMonth](),
            minDateDate = minDate[getDate](),
            timezoneOffset = useUTC ?
                0 :
                (24 * 3600 * 1000 + minDate.getTimezoneOffset() * 60 * 1000) % (24 * 3600 * 1000); // #950
        // iterate and add tick positions at appropriate values
        while (time < max) {
            tickPositions.push(time);
            // if the interval is years, use Date.UTC to increase years
            if (interval === timeUnits[YEAR]) {
                time = makeTime(minYear + i * count, 0);
            // if the interval is months, use Date.UTC to increase months
            } else if (interval === timeUnits[MONTH]) {
                time = makeTime(minYear, minMonth + i * count);
            // if we're using global time, the interval is not fixed as it jumps
            // one hour at the DST crossover
            } else if (!useUTC && (interval === timeUnits[DAY] || interval === timeUnits[WEEK])) {
                time = makeTime(minYear, minMonth, minDateDate +
                    i * count * (interval === timeUnits[DAY] ? 1 : 7));
            // else, the interval is fixed and we use simple addition
            } else {
                time += interval * count;
                // mark new days if the time is dividable by day
                if (interval <= timeUnits[HOUR] && time % timeUnits[DAY] === timezoneOffset) {
                    higherRanks[time] = DAY;
                }
            }
            i++;
        }
        // push the last time
        tickPositions.push(time);
    }
    // record information on the chosen unit - for dynamic label formatter
    tickPositions.info = extend(normalizedInterval, {
        higherRanks: higherRanks,
        totalRange: interval * count
    });
    return tickPositions;
}
/**
 * Helper class that contains variuos counters that are local to the chart.
 */
function ChartCounters() {
    this.color = 0;
    this.symbol = 0;
}
ChartCounters.prototype =  {
    /**
     * Wraps the color counter if it reaches the specified length.
     */
    wrapColor: function (length) {
        if (this.color >= length) {
            this.color = 0;
        }
    },
    /**
     * Wraps the symbol counter if it reaches the specified length.
     */
    wrapSymbol: function (length) {
        if (this.symbol >= length) {
            this.symbol = 0;
        }
    }
};
/**
 * Utility method that sorts an object array and keeping the order of equal items.
 * ECMA script standard does not specify the behaviour when items are equal.
 */
function stableSort(arr, sortFunction) {
    let length = arr.length,
        sortValue,
        i;
    // Add index to each item
    for (i = 0; i < length; i++) {
        arr[i].ss_i = i; // stable sort index
    }
    arr.sort(function (a, b) {
        sortValue = sortFunction(a, b);
        return sortValue === 0 ? a.ss_i - b.ss_i : sortValue;
    });
    // Remove index from items
    for (i = 0; i < length; i++) {
        delete arr[i].ss_i; // stable sort index
    }
}
/**
 * Non-recursive method to find the lowest member of an array. Math.min raises a maximum
 * call stack size exceeded error in Chrome when trying to apply more than 150.000 points. This
 * method is slightly slower, but safe.
 */
function arrayMin(data) {
    let i = data.length,
        min = data[0];
    while (i--) {
        if (data[i] < min) {
            min = data[i];
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
    let i = data.length,
        max = data[0];
    while (i--) {
        if (data[i] > max) {
            max = data[i];
        }
    }
    return max;
}
/**
 * Utility method that destroys any SVGElement or VMLElement that are properties on the given object.
 * It loops all properties and invokes destroy if there is a destroy method. The property is
 * then delete'ed.
 * @param {Object} The object to destroy properties on
 * @param {Object} Exception, do not destroy this property, only delete it.
 */
function destroyObjectProperties(obj, except) {
    let n;
    for (n in obj) {
        // If the object is non-null and destroy is defined
        if (obj[n] && obj[n] !== except && obj[n].destroy) {
            // Invoke the destroy
            obj[n].destroy();
        }
        // Delete the property from the object.
        delete obj[n];
    }
}
/**
 * Discard an element by moving it to the bin and delete
 * @param {Object} The HTML node to discard
 */
function discardElement(element) {
    // create a garbage bin element, not part of the DOM
    if (!garbageBin) {
        garbageBin = createElement(DIV);
    }
    // move the node and empty bin
    if (element) {
        garbageBin.appendChild(element);
    }
    garbageBin.innerHTML = '';
}
/**
 * Provide error messages for debugging, with links to online explanation
 */
function error(code, stop) {
    let msg = 'Highcharts error #' + code + ': www.highcharts.com/errors/' + code;
    if (stop) {
        throw msg;
    } else if (win.console) {
        console.log(msg);
    }
}
/**
 * Fix JS round off float errors
 * @param {Number} num
 */
function correctFloat(num) {
    return parseFloat(
        num.toPrecision(14)
    );
}
/**
 * The time unit lookup
 */
/*jslint white: true*/
timeUnits = hash(
    MILLISECOND, 1,
    SECOND, 1000,
    MINUTE, 60000,
    HOUR, 3600000,
    DAY, 24 * 3600000,
    WEEK, 7 * 24 * 3600000,
    MONTH, 31 * 24 * 3600000,
    YEAR, 31556952000
);
/*jslint white: false*/
/**
 * Path interpolation algorithm used across adapters
 */
pathAnim = {
    /**
     * Prepare start and end values so that the path can be animated one to one
     */
    init: function (elem, fromD, toD) {
        fromD = fromD || '';
        let shift = elem.shift,
            bezier = fromD.indexOf('C') > -1,
            numParams = bezier ? 7 : 3,
            endLength,
            slice,
            i,
            start = fromD.split(' '),
            end = [].concat(toD), // copy
            startBaseLine,
            endBaseLine,
            sixify = function (arr) { // in splines make move points have six parameters like bezier curves
                i = arr.length;
                while (i--) {
                    if (arr[i] === M) {
                        arr.splice(i + 1, 0, arr[i + 1], arr[i + 2], arr[i + 1], arr[i + 2]);
                    }
                }
            };
        if (bezier) {
            sixify(start);
            sixify(end);
        }
        // pull out the base lines before padding
        if (elem.isArea) {
            startBaseLine = start.splice(start.length - 6, 6);
            endBaseLine = end.splice(end.length - 6, 6);
        }
        // if shifting points, prepend a dummy point to the end path
        if (shift <= end.length / numParams) {
            while (shift--) {
                end = [].concat(end).splice(0, numParams).concat(end);
            }
        }
        elem.shift = 0; // reset for following animations
        // copy and append last point until the length matches the end length
        if (start.length) {
            endLength = end.length;
            while (start.length < endLength) {
                //bezier && sixify(start);
                slice = [].concat(start).splice(start.length - numParams, numParams);
                if (bezier) { // disable first control point
                    slice[numParams - 6] = slice[numParams - 2];
                    slice[numParams - 5] = slice[numParams - 1];
                }
                start = start.concat(slice);
            }
        }
        if (startBaseLine) { // append the base lines for areas
            start = start.concat(startBaseLine);
            end = end.concat(endBaseLine);
        }
        return [start, end];
    },
    /**
     * Interpolate each value of the path and return the array
     */
    step: function (start, end, pos, complete) {
        let ret = [],
            i = start.length,
            startVal;
        if (pos === 1) { // land on the final path without adjustment points appended in the ends
            ret = complete;
        } else if (i === end.length && pos < 1) {
            while (i--) {
                startVal = parseFloat(start[i]);
                ret[i] =
                    isNaN(startVal) ? // a letter instruction like M or L
                        start[i] :
                        pos * (parseFloat(end[i] - startVal)) + startVal;
            }
        } else { // if animation is finished or length not matching, land on right value
            ret = end;
        }
        return ret;
    }
};
/**
 * The default HighchartsAdapter for jQuery
 */
win.HighchartsAdapter = {
    /**
     * Initialize the adapter by applying some extensions to jQuery
     */
    init: function (pathAnim) {
        // extend the animate function to allow SVG animations
        let Fx = jQuery.fx,
            Step = Fx.step,
            dSetter,
            Tween = jQuery.Tween,
            propHooks = Tween && Tween.propHooks;
        /*jslint unparam: false*/
        // extend some methods to check for elem.attr, which means it is a Highcharts SVG object
        jQuery.each(['cur', '_default', 'width', 'height'], function (i, fn) {
            let obj = Step,
                base,
                elem;
            // Handle different parent objects
            if (fn === 'cur') {
                obj = Fx.prototype; // 'cur', the getter, relates to Fx.prototype
            } else if (fn === '_default' && Tween) { // jQuery 1.8 model
                obj = propHooks[fn];
                fn = 'set';
            }
            // Overwrite the method
            base = obj[fn];
            if (base) { // step.width and step.height don't exist in jQuery < 1.7
                // create the extended function replacement
                obj[fn] = function (fx) {
                    // Fx.prototype.cur does not use fx argument
                    fx = i ? fx : this;
                    // shortcut
                    elem = fx.elem;
                    // Fx.prototype.cur returns the current value. The other ones are setters
                    // and returning a value has no effect.
                    return elem.attr ? // is SVG element wrapper
                        elem.attr(fx.prop, fn === 'cur' ? UNDEFINED : fx.now) : // apply the SVG wrapper's method
                        base.apply(this, arguments); // use jQuery's built-in method
                };
            }
        });
        // Define the setter function for d (path definitions)
        dSetter = function (fx) {
            let elem = fx.elem,
                ends;
            // Normally start and end should be set in state == 0, but sometimes,
            // for reasons unknown, this doesn't happen. Perhaps state == 0 is skipped
            // in these cases
            if (!fx.started) {
                ends = pathAnim.init(elem, elem.d, elem.toD);
                fx.start = ends[0];
                fx.end = ends[1];
                fx.started = true;
            }
            // interpolate each value of the path
            elem.attr('d', pathAnim.step(fx.start, fx.end, fx.pos, elem.toD));
        };
        // jQuery 1.8 style
        if (Tween) {
            propHooks.d = {
                set: dSetter
            };
        // pre 1.8
        } else {
            // animate paths
            Step.d = dSetter;
        }
        /**
         * Utility for iterating over an array. Parameters are reversed compared to jQuery.
         * @param {Array} arr
         * @param {Function} fn
         */
        this.each = Array.prototype.forEach ?
            function (arr, fn) { // modern browsers
                return Array.prototype.forEach.call(arr, fn);
            } :
            function (arr, fn) { // legacy
                let i = 0,
                    len = arr.length;
                for (; i < len; i++) {
                    if (fn.call(arr[i], arr[i], i, arr) === false) {
                        return i;
                    }
                }
            };
        // Register Highcharts as a jQuery plugin
        // TODO: MooTools and prototype as well?
        // TODO: StockChart
        /*jQuery.fn.highcharts = function(options, callback) {
            options.chart = merge(options.chart, { renderTo: this[0] });
            this.chart = new Chart(options, callback);
            return this;
        };*/
    },
    /**
     * Downloads a script and executes a callback when done.
     * @param {String} scriptLocation
     * @param {Function} callback
     */
    getScript: jQuery.getScript,
    /**
     * Return the index of an item in an array, or -1 if not found
     */
    inArray: jQuery.inArray,
    /**
     * A direct link to jQuery methods. MooTools and Prototype adapters must be implemented for each case of method.
     * @param {Object} elem The HTML element
     * @param {String} method Which method to run on the wrapped element
     */
    adapterRun: function (elem, method) {
        return jQuery(elem)[method]();
    },
    /**
     * Filter an array
     */
    grep: jQuery.grep,
    /**
     * Map an array
     * @param {Array} arr
     * @param {Function} fn
     */
    map: function (arr, fn) {
        //return jQuery.map(arr, fn);
        let results = [],
            i = 0,
            len = arr.length;
        for (; i < len; i++) {
            results[i] = fn.call(arr[i], arr[i], i, arr);
        }
        return results;
    },
    /**
     * Deep merge two objects and return a third object
     */
    merge: function () {
        let args = arguments;
        return jQuery.extend(true, null, args[0], args[1], args[2], args[3]);
    },
    /**
     * Get the position of an element relative to the top left of the page
     */
    offset: function (el) {
        return jQuery(el).offset();
    },
    /**
     * Add an event listener
     * @param {Object} el A HTML element or custom object
     * @param {String} event The event type
     * @param {Function} fn The event handler
     */
    addEvent: function (el, event, fn) {
        jQuery(el).bind(event, fn);
    },
    /**
     * Remove event added with addEvent
     * @param {Object} el The object
     * @param {String} eventType The event type. Leave blank to remove all events.
     * @param {Function} handler The function to remove
     */
    removeEvent: function (el, eventType, handler) {
        // workaround for jQuery issue with unbinding custom events:
        // http://forum.jQuery.com/topic/javascript-error-when-unbinding-a-custom-event-using-jQuery-1-4-2
        let func = doc.removeEventListener ? 'removeEventListener' : 'detachEvent';
        if (doc[func] && !el[func]) {
            el[func] = function () {};
        }
        jQuery(el).unbind(eventType, handler);
    },
    /**
     * Fire an event on a custom object
     * @param {Object} el
     * @param {String} type
     * @param {Object} eventArguments
     * @param {Function} defaultFunction
     */
    fireEvent: function (el, type, eventArguments, defaultFunction) {
        let event = jQuery.Event(type),
            detachedType = 'detached' + type,
            defaultPrevented;
        // Remove warnings in Chrome when accessing layerX and layerY. Although Highcharts
        // never uses these properties, Chrome includes them in the default click event and
        // raises the warning when they are copied over in the extend statement below.
        //
        // To avoid problems in IE (see #1010) where we cannot delete the properties and avoid
        // testing if they are there (warning in chrome) the only option is to test if running IE.
        if (!isIE && eventArguments) {
            delete eventArguments.layerX;
            delete eventArguments.layerY;
        }
        extend(event, eventArguments);
        // Prevent jQuery from triggering the object method that is named the
        // same as the event. For example, if the event is 'select', jQuery
        // attempts calling el.select and it goes into a loop.
        if (el[type]) {
            el[detachedType] = el[type];
            el[type] = null;
        }
        // Wrap preventDefault and stopPropagation in try/catch blocks in
        // order to prevent JS errors when cancelling events on non-DOM
        // objects. #615.
        /*jslint unparam: true*/
        jQuery.each(['preventDefault', 'stopPropagation'], function (i, fn) {
            let base = event[fn];
            event[fn] = function () {
                try {
                    base.call(event);
                } catch (e) {
                    if (fn === 'preventDefault') {
                        defaultPrevented = true;
                    }
                }
            };
        });
        /*jslint unparam: false*/
        // trigger it
        jQuery(el).trigger(event);
        // attach the method
        if (el[detachedType]) {
            el[type] = el[detachedType];
            el[detachedType] = null;
        }
        if (defaultFunction && !event.isDefaultPrevented() && !defaultPrevented) {
            defaultFunction(event);
        }
    },
    /**
     * Extension method needed for MooTools
     */
    washMouseEvent: function (e) {
        let ret = e.originalEvent || e;
        // computed by jQuery, needed by IE8
        if (ret.pageX === UNDEFINED) { // #1236
            ret.pageX = e.pageX;
            ret.pageY = e.pageY;
        }
        return ret;
    },
    /**
     * Animate a HTML element or SVG element wrapper
     * @param {Object} el
     * @param {Object} params
     * @param {Object} options jQuery-like animation options: duration, easing, callback
     */
    animate: function (el, params, options) {
        let $el = jQuery(el);
        if (params.d) {
            el.toD = params.d; // keep the array form for paths, used in jQuery.fx.step.d
            params.d = 1; // because in jQuery, animating to an array has a different meaning
        }
        $el.stop();
        $el.animate(params, options);
    },
    /**
     * Stop running animation
     */
    stop: function (el) {
        jQuery(el).stop();
    }
};
// check for a custom HighchartsAdapter defined prior to this file
let globalAdapter = win.HighchartsAdapter,
    adapter = globalAdapter || {};
// Initialize the adapter
if (globalAdapter) {
    globalAdapter.init.call(globalAdapter, pathAnim);
}
    // Utility functions. If the HighchartsAdapter is not defined, adapter is an empty object
    // and all the utility functions will be null. In that case they are populated by the
    // default adapters below.
let adapterRun = adapter.adapterRun,
    getScript = adapter.getScript,
    inArray = adapter.inArray,
    each = adapter.each,
    grep = adapter.grep,
    offset = adapter.offset,
    map = adapter.map,
    merge = adapter.merge,
    addEvent = adapter.addEvent,
    removeEvent = adapter.removeEvent,
    fireEvent = adapter.fireEvent,
    washMouseEvent = adapter.washMouseEvent,
    animate = adapter.animate,
    stop = adapter.stop;
/* ****************************************************************************
 * Handle the options                                                         *
 *****************************************************************************/
var
defaultLabelOptions = {
    enabled: true,
    // rotation: 0,
    align: 'center',
    x: 0,
    y: 15,
    /*formatter: function () {
        return this.value;
    },*/
    style: {
        color: '#666',
        fontSize: '11px',
        lineHeight: '14px'
    }
};
defaultOptions = {
    colors: ['#4572A7', '#AA4643', '#89A54E', '#80699B', '#3D96AE',
        '#DB843D', '#92A8CD', '#A47D7C', '#B5CA92'],
    symbols: ['circle', 'diamond', 'square', 'triangle', 'triangle-down'],
    lang: {
        loading: 'Loading...',
        months: ['January', 'February', 'March', 'April', 'May', 'June', 'July',
                'August', 'September', 'October', 'November', 'December'],
        shortMonths: ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec'],
        weekdays: ['Sunday', 'Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday'],
        decimalPoint: '.',
        numericSymbols: ['k', 'M', 'G', 'T', 'P', 'E'], // SI prefixes used in axis labels
        thousandsSep: ','
    },
    global: {
        useUTC: true,
        canvasToolsURL: 'http://code.highcharts.com/2.3.5/modules/canvas-tools.js',
    },
    chart: {
        //animation: true,
        //alignTicks: false,
        //reflow: true,
        //className: null,
        //events: { load, selection },
        //margin: [null],
        //marginTop: null,
        //marginLeft: null,
        borderColor: '#4572A7',
        //borderWidth: 0,
        borderRadius: 5,
        defaultSeriesType: 'line',
        ignoreHiddenSeries: true,
        //shadow: false,
        spacingTop: 10,
        spacingRight: 10,
        spacingBottom: 15,
        spacingLeft: 10,
        backgroundColor: '#FFFFFF',
        plotBorderColor: '#C0C0C0',
        //plotBorderWidth: 0,
        //plotShadow: false,
    },
    plotOptions: {
        line: { // base series options
            allowPointSelect: false,
            showCheckbox: false,
            animation: {
                duration: 1000
            },
            //connectNulls: false,
            //cursor: 'default',
            //clip: true,
            //dashStyle: null,
            //enableMouseTracking: true,
            events: {},
            lineWidth: 2,
            shadow: true,
            marker: {
                enabled: true,
                //symbol: null,
                lineWidth: 0,
                radius: 4,
                lineColor: '#FFFFFF',
                //fillColor: null,
                states: { // states for a single point
                    hover: {
                        enabled: true
                        //radius: base + 2
                    },
                    select: {
                        fillColor: '#FFFFFF',
                        lineColor: '#000000',
                        lineWidth: 2
                    }
                }
            },
            point: {
                events: {}
            },
            dataLabels: merge(defaultLabelOptions, {
                enabled: false,
                formatter: function () {
                    return this.y;
                },
                verticalAlign: 'bottom', // above singular point
                y: 0
                // backgroundColor: undefined,
                // borderColor: undefined,
                // borderRadius: undefined,
                // borderWidth: undefined,
                // padding: 3,
                // shadow: false
            }),
            cropThreshold: 300, // draw points outside the plot area when the number of points is less than this
            pointRange: 0,
            //pointStart: 0,
            //pointInterval: 1,
            states: { // states for the entire series
                hover: {
                    //enabled: false,
                    //lineWidth: base + 1,
                    marker: {
                        // lineWidth: base + 1,
                        // radius: base + 1
                    }
                },
                select: {
                    marker: {}
                }
            },
            stickyTracking: true
            //tooltip: {
                //pointFormat: '<span style="color:{series.color}">{series.name}</span>: <b>{point.y}</b>'
                //valueDecimals: null,
                //xDateFormat: '%A, %b %e, %Y',
                //valuePrefix: '',
                //ySuffix: ''
            //}
            // zIndex: null
        }
    },
    labels: {
        //items: [],
        style: {
            //font: defaultFont,
            position: ABSOLUTE,
            color: '#3E576F'
        }
    },
    loading: {
        // hideDuration: 100,
        labelStyle: {
            fontWeight: 'bold',
            position: RELATIVE,
            top: '1em'
        },
        // showDuration: 0,
        style: {
            position: ABSOLUTE,
            backgroundColor: 'white',
            opacity: 0.5,
            textAlign: 'center'
        }
    },
    tooltip: {
        enabled: true,
        //crosshairs: null,
        backgroundColor: 'rgba(255, 255, 255, .85)',
        borderWidth: 2,
        borderRadius: 5,
        dateTimeLabelFormats: {
            millisecond: '%A, %b %e, %H:%M:%S.%L',
            second: '%A, %b %e, %H:%M:%S',
            minute: '%A, %b %e, %H:%M',
            hour: '%A, %b %e, %H:%M',
            day: '%A, %b %e, %Y',
            week: 'Week from %A, %b %e, %Y',
            month: '%B %Y',
            year: '%Y'
        },
        //formatter: defaultFormatter,
        headerFormat: '<span style="font-size: 10px">{point.key}</span><br/>',
        pointFormat: '<span style="color:{series.color}">{series.name}</span>: <b>{point.y}</b><br/>',
        shadow: true,
        snap: isTouchDevice ? 25 : 10,
        style: {
            color: '#333333',
            fontSize: '12px',
            padding: '5px',
            whiteSpace: 'nowrap'
        }
        //xDateFormat: '%A, %b %e, %Y',
        //valueDecimals: null,
        //valuePrefix: '',
        //valueSuffix: ''
    }
};
// Series defaults
let defaultPlotOptions = defaultOptions.plotOptions,
    defaultSeriesOptions = defaultPlotOptions.line;
// set the default time methods
setTimeMethods();
/**
 * Set the time methods globally based on the useUTC option. Time method can be either
 * local time or UTC (default).
 */
function setTimeMethods() {
    let useUTC = defaultOptions.global.useUTC,
        GET = useUTC ? 'getUTC' : 'get',
        SET = useUTC ? 'setUTC' : 'set';
    makeTime = useUTC ? Date.UTC : function (year, month, date, hours, minutes, seconds) {
        return new Date(
            year,
            month,
            pick(date, 1),
            pick(hours, 0),
            pick(minutes, 0),
            pick(seconds, 0)
        ).getTime();
    };
    getMinutes =  GET + 'Minutes';
    getHours =    GET + 'Hours';
    getDay =      GET + 'Day';
    getDate =     GET + 'Date';
    getMonth =    GET + 'Month';
    getFullYear = GET + 'FullYear';
    setMinutes =  SET + 'Minutes';
    setHours =    SET + 'Hours';
    setDate =     SET + 'Date';
    setMonth =    SET + 'Month';
    setFullYear = SET + 'FullYear';
}
/**
 * Get the updated default options. Merely exposing defaultOptions for outside modules
 * isn't enough because the setOptions method creates a new object.
 */
function getOptions() {
    return defaultOptions;
}
/**
 * Handle color operations. The object methods are chainable.
 * @param {String} input The input color in either rbga or hex format
 */
let Color = function (input) {
    // declare variables
    let rgba = [], result;
    /**
     * Parse the input color to rgba array
     * @param {String} input
     */
    function init(input) {
        // rgba
        result = /rgba\(\s*([0-9]{1,3})\s*,\s*([0-9]{1,3})\s*,\s*([0-9]{1,3})\s*,\s*([0-9]?(?:\.[0-9]+)?)\s*\)/.exec(input);
        if (result) {
            rgba = [pInt(result[1]), pInt(result[2]), pInt(result[3]), parseFloat(result[4], 10)];
        } else { // hex
            result = /#([a-fA-F0-9]{2})([a-fA-F0-9]{2})([a-fA-F0-9]{2})/.exec(input);
            if (result) {
                rgba = [pInt(result[1], 16), pInt(result[2], 16), pInt(result[3], 16), 1];
            }
        }
    }
    /**
     * Return the color a specified format
     * @param {String} format
     */
    function get(format) {
        let ret;
        // it's NaN if gradient colors on a column chart
        if (rgba && !isNaN(rgba[0])) {
            if (format === 'rgb') {
                ret = 'rgb(' + rgba[0] + ',' + rgba[1] + ',' + rgba[2] + ')';
            } else if (format === 'a') {
                ret = rgba[3];
            } else {
                ret = 'rgba(' + rgba.join(',') + ')';
            }
        } else {
            ret = input;
        }
        return ret;
    }
    /**
     * Set the color's opacity to a given alpha value
     * @param {Number} alpha
     */
    function setOpacity(alpha) {
        rgba[3] = alpha;
        return this;
    }
    // initialize: parse the input
    init(input);
    // public methods
    return {
        get: get,
        setOpacity: setOpacity
    };
};
/**
 * A wrapper object for SVG elements
 */
function SVGElement() {}
SVGElement.prototype = {
    /**
     * Initialize the SVG renderer
     * @param {Object} renderer
     * @param {String} nodeName
     */
    init: function (renderer, nodeName) {
        let wrapper = this;
        wrapper.element = nodeName === 'span' ?
            createElement(nodeName) :
            doc.createElementNS(SVG_NS, nodeName);
        wrapper.renderer = renderer;
        /**
         * A collection of attribute setters. These methods, if defined, are called right before a certain
         * attribute is set on an element wrapper. Returning false prevents the default attribute
         * setter to run. Returning a value causes the default setter to set that value. Used in
         * Renderer.label.
         */
        wrapper.attrSetters = {};
    },
    /**
     * Animate a given attribute
     * @param {Object} params
     * @param {Number} options The same options as in jQuery animation
     * @param {Function} complete Function to perform at the end of animation
     */
    animate: function (params, options, complete) {
        let animOptions = pick(options, true);
        stop(this); // stop regardless of animation actually running, or reverting to .attr (#607)
        if (animOptions) {
            animOptions = merge(animOptions);
            if (complete) { // allows using a callback with the global animation without overwriting it
                animOptions.complete = complete;
            }
            animate(this, params, animOptions);
        } else {
            this.attr(params);
            if (complete) {
                complete();
            }
        }
    },
    /**
     * Set or get a given attribute
     * @param {Object|String} hash
     * @param {Mixed|Undefined} val
     */
    attr: function (hash, val) {
        let wrapper = this,
            key,
            value,
            result,
            i,
            child,
            element = wrapper.element,
            nodeName = element.nodeName.toLowerCase(), // Android2 requires lower for "text"
            renderer = wrapper.renderer,
            skipAttr,
            attrSetters = wrapper.attrSetters,
            shadows = wrapper.shadows,
            hasSetSymbolSize,
            doTransform,
            ret = wrapper;
        // single key-value pair
        if (isString(hash) && defined(val)) {
            key = hash;
            hash = {};
            hash[key] = val;
        }
        // used as a getter: first argument is a string, second is undefined
        if (isString(hash)) {
            key = hash;
            if (nodeName === 'circle') {
                key = { x: 'cx', y: 'cy' }[key] || key;
            } else if (key === 'strokeWidth') {
                key = 'stroke-width';
            }
            ret = attr(element, key) || wrapper[key] || 0;
            if (key !== 'd' && key !== 'visibility') { // 'd' is string in animation step
                ret = parseFloat(ret);
            }
        // setter
        } else {
            for (key in hash) {
                skipAttr = false; // reset
                value = hash[key];
                // check for a specific attribute setter
                result = attrSetters[key] && attrSetters[key].call(wrapper, value, key);
                if (result !== false) {
                    if (result !== UNDEFINED) {
                        value = result; // the attribute setter has returned a new value to set
                    }
                    // paths
                    if (key === 'd') {
                        if (value && value.join) { // join path
                            value = value.join(' ');
                        }
                        if (/(NaN| {2}|^$)/.test(value)) {
                            value = 'M 0 0';
                        }
                        //wrapper.d = value; // shortcut for animations
                    // update child tspans x values
                    } else if (key === 'x' && nodeName === 'text') {
                        for (i = 0; i < element.childNodes.length; i++) {
                            child = element.childNodes[i];
                            // if the x values are equal, the tspan represents a linebreak
                            if (attr(child, 'x') === attr(element, 'x')) {
                                //child.setAttribute('x', value);
                                attr(child, 'x', value);
                            }
                        }
                        if (wrapper.rotation) {
                            attr(element, 'transform', 'rotate(' + wrapper.rotation + ' ' + value + ' ' +
                                pInt(hash.y || attr(element, 'y')) + ')');
                        }
                    // apply gradients
                    } else if (key === 'fill') {
                        value = renderer.color(value, element, key);
                    // circle x and y
                    } else if (nodeName === 'circle' && (key === 'x' || key === 'y')) {
                        key = { x: 'cx', y: 'cy' }[key] || key;
                    // rectangle border radius
                    } else if (nodeName === 'rect' && key === 'r') {
                        attr(element, {
                            rx: value,
                            ry: value
                        });
                        skipAttr = true;
                    // translation and text rotation
                    } else if (key === 'translateX' || key === 'translateY' || key === 'rotation' || key === 'verticalAlign') {
                        doTransform = true;
                        skipAttr = true;
                    // apply opacity as subnode (required by legacy WebKit and Batik)
                    } else if (key === 'stroke') {
                        value = renderer.color(value, element, key);
                    // emulate VML's dashstyle implementation
                    } else if (key === 'dashstyle') {
                        key = 'stroke-dasharray';
                        value = value && value.toLowerCase();
                        if (value === 'solid') {
                            value = NONE;
                        } else if (value) {
                            value = value
                                .replace('shortdashdotdot', '3,1,1,1,1,1,')
                                .replace('shortdashdot', '3,1,1,1')
                                .replace('shortdot', '1,1,')
                                .replace('shortdash', '3,1,')
                                .replace('longdash', '8,3,')
                                .replace(/dot/g, '1,3,')
                                .replace('dash', '4,3,')
                                .replace(/,$/, '')
                                .split(','); // ending comma
                            i = value.length;
                            while (i--) {
                                value[i] = pInt(value[i]) * hash['stroke-width'];
                            }
                            value = value.join(',');
                        }
                    // special
                    } else if (key === 'isTracker') {
                        wrapper[key] = value;
                    // IE9/MooTools combo: MooTools returns objects instead of numbers and IE9 Beta 2
                    // is unable to cast them. Test again with final IE9.
                    } else if (key === 'width') {
                        value = pInt(value);
                    // Text alignment
                    } else if (key === 'align') {
                        key = 'text-anchor';
                        value = { left: 'start', center: 'middle', right: 'end' }[value];
                    }
                    // jQuery animate changes case
                    if (key === 'strokeWidth') {
                        key = 'stroke-width';
                    }
                    // Chrome/Win < 6 bug (http://code.google.com/p/chromium/issues/detail?id=15461), #1369
                    if (key === 'stroke-width' && value === 0 && (isWebKit || renderer.forExport)) {
                        value = 0.000001;
                    }
                    // symbols
                    if (wrapper.symbolName && /^(x|y|width|height|r|start|end|innerR|anchorX|anchorY)/.test(key)) {
                        if (!hasSetSymbolSize) {
                            wrapper.symbolAttr(hash);
                            hasSetSymbolSize = true;
                        }
                        skipAttr = true;
                    }
                    // let the shadow follow the main element
                    if (shadows && /^(width|height|visibility|x|y|d|transform)$/.test(key)) {
                        i = shadows.length;
                        while (i--) {
                            attr(
                                shadows[i],
                                key,
                                key === 'height' ?
                                    mathMax(value - (shadows[i].cutHeight || 0), 0) :
                                    value
                            );
                        }
                    }
                    // validate heights
                    if ((key === 'width' || key === 'height') && nodeName === 'rect' && value < 0) {
                        value = 0;
                    }
                    // Record for animation and quick access without polling the DOM
                    wrapper[key] = value;
                    // Update transform
                    if (doTransform) {
                        wrapper.updateTransform();
                    }
                    if (key === 'text') {
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
                }
            }
        }
        return ret;
    },
    /**
     * If one of the symbol size affecting parameters are changed,
     * check all the others only once for each call to an element's
     * .attr() method
     * @param {Object} hash
     */
    symbolAttr: function (hash) {
        let wrapper = this;
        each(['x', 'y', 'r', 'start', 'end', 'width', 'height', 'innerR', 'anchorX', 'anchorY'], function (key) {
            wrapper[key] = pick(hash[key], wrapper[key]);
        });
        wrapper.attr({
            d: wrapper.renderer.symbols[wrapper.symbolName](wrapper.x, wrapper.y, wrapper.width, wrapper.height, wrapper)
        });
    },
    /**
     * Apply a clipping path to this object
     * @param {String} id
     */
    clip: function (clipRect) {
        return this.attr('clip-path', clipRect ? 'url(' + this.renderer.url + '#' + clipRect.id + ')' : NONE);
    },
    /**
     * Calculate the coordinates needed for drawing a rectangle crisply and return the
     * calculated attributes
     * @param {Number} strokeWidth
     * @param {Number} x
     * @param {Number} y
     * @param {Number} width
     * @param {Number} height
     */
    crisp: function (strokeWidth, x, y, width, height) {
        let wrapper = this,
            key,
            attribs = {},
            values = {},
            normalizer;
        strokeWidth = strokeWidth || wrapper.strokeWidth || (wrapper.attr && wrapper.attr('stroke-width')) || 0;
        normalizer = mathRound(strokeWidth) % 2 / 2; // mathRound because strokeWidth can sometimes have roundoff errors
        // normalize for crisp edges
        values.x = mathFloor(x || wrapper.x || 0) + normalizer;
        values.y = mathFloor(y || wrapper.y || 0) + normalizer;
        values.width = mathFloor((width || wrapper.width || 0) - 2 * normalizer);
        values.height = mathFloor((height || wrapper.height || 0) - 2 * normalizer);
        values.strokeWidth = strokeWidth;
        for (key in values) {
            if (wrapper[key] !== values[key]) { // only set attribute if changed
                wrapper[key] = attribs[key] = values[key];
            }
        }
        return attribs;
    },
    /**
     * Set styles for the element
     * @param {Object} styles
     */
    css: function (styles) {
        /*jslint unparam: true*//* allow unused param a in the regexp function below */
        let elemWrapper = this,
            elem = elemWrapper.element,
            textWidth = styles && styles.width && elem.nodeName.toLowerCase() === 'text',
            n,
            serializedCss = '',
            hyphenate = function (a, b) { return '-' + b.toLowerCase(); };
        /*jslint unparam: false*/
        // convert legacy
        if (styles && styles.color) {
            styles.fill = styles.color;
        }
        // Merge the new styles with the old ones
        styles = extend(
            elemWrapper.styles,
            styles
        );
        // store object
        elemWrapper.styles = styles;
        for (n in styles) {
            serializedCss += n.replace(/([A-Z])/g, hyphenate) + ':' + styles[n] + ';';
        }
        elemWrapper.attr({
            style: serializedCss
        });
        // re-build text
        if (textWidth && elemWrapper.added) {
            elemWrapper.renderer.buildText(elemWrapper);
        }
        return elemWrapper;
    },
    /**
     * Add an event listener
     * @param {String} eventType
     * @param {Function} handler
     */
    on: function (eventType, handler) {
        // touch
        if (hasTouch && eventType === 'click') {
            this.element.ontouchstart = function (e) {
                e.preventDefault();
                handler();
            };
        }
        // simplest possible event model for internal use
        this.element['on' + eventType] = handler;
        return this;
    },
    /**
     * Set the coordinates needed to draw a consistent radial gradient across
     * pie slices regardless of positioning inside the chart. The format is
     * [centerX, centerY, diameter] in pixels.
     */
    setRadialReference: function (coordinates) {
        this.element.radialReference = coordinates;
        return this;
    },
    /**
     * Move an object and its children by x and y values
     * @param {Number} x
     * @param {Number} y
     */
    translate: function (x, y) {
        return this.attr({
            translateX: x,
            translateY: y
        });
    },
    /**
     * VML and useHTML method for calculating the bounding box based on offsets
     * @param {Boolean} refresh Whether to force a fresh value from the DOM or to
     * use the cached value
     *
     * @return {Object} A hash containing values for x, y, width and height
     */
    htmlGetBBox: function () {
        let wrapper = this,
            element = wrapper.element,
            bBox = wrapper.bBox;
        return bBox;
    },
    /**
     * Private method to update the transform attribute based on internal
     * properties
     */
    updateTransform: function () {
        let wrapper = this,
            translateX = wrapper.translateX || 0,
            translateY = wrapper.translateY || 0,
            rotation = wrapper.rotation,
            transform = [];
        // flipping affects translate as adjustment for flipping around the group's axis
        // apply translate
        if (translateX || translateY) {
            transform.push('translate(' + translateX + ',' + translateY + ')');
        }
        // apply rotation
        if (rotation) { // text rotation
            transform.push('rotate(' + rotation + ' ' + (wrapper.x || 0) + ' ' + (wrapper.y || 0) + ')');
        }
        if (transform.length) {
            attr(wrapper.element, 'transform', transform.join(' '));
        }
    },
    /**
     * Get the bounding box (width, height, x and y) for the element
     */
    getBBox: function () {
        let wrapper = this,
            bBox = wrapper.bBox,
            renderer = wrapper.renderer,
            width,
            height,
            rotation = wrapper.rotation,
            element = wrapper.element,
            styles = wrapper.styles,
            rad = rotation * deg2rad;
        if (!bBox) {
            // SVG elements
            if (element.namespaceURI === SVG_NS || renderer.forExport) {
                bBox =
                    // SVG: use extend because IE9 is not allowed to change width and height in case
                    // of rotation (below)
                    extend({}, element.getBBox());
                // If the bBox is not set, the try-catch block above failed. The other condition
                // is for Opera that returns a width of -Infinity on hidden elements.
                if (!bBox || bBox.width < 0) {
                    bBox = { width: 0, height: 0 };
                }
            // VML Renderer or useHTML within SVG
            } else {
                bBox = wrapper.htmlGetBBox();
            }
            // True SVG elements as well as HTML elements in modern browsers using the .useHTML option
            // need to compensated for rotation
            if (renderer.isSVG) {
                width = bBox.width;
                height = bBox.height;
                // Adjust for rotated text
                if (rotation) {
                    bBox.width = mathAbs(height * mathSin(rad)) + mathAbs(width * mathCos(rad));
                    bBox.height = mathAbs(height * mathCos(rad)) + mathAbs(width * mathSin(rad));
                }
            }
            wrapper.bBox = bBox;
        }
        return bBox;
    },
    /**
     * Show the element
     */
    show: function () {
        return this.attr({ visibility: VISIBLE });
    },
    /**
     * Hide the element
     */
    hide: function () {
        return this.attr({ visibility: HIDDEN });
    },
    /**
     * Add the element
     * @param {Object|Undefined} parent Can be an element, an element wrapper or undefined
     *    to append the element to the renderer.box.
     */
    add: function (parent) {
        let renderer = this.renderer,
            parentWrapper = parent || renderer,
            parentNode = parentWrapper.element || renderer.box,
            childNodes = parentNode.childNodes,
            element = this.element,
            zIndex = attr(element, 'zIndex'),
            otherElement,
            otherZIndex,
            i,
            inserted;
        if (parent) {
            this.parentGroup = parent;
        }
        // build formatted text
        if (this.textStr !== undefined) {
            renderer.buildText(this);
        }
        // mark the container as having z indexed children
        if (zIndex) {
            parentWrapper.handleZ = true;
            zIndex = pInt(zIndex);
        }
        // insert according to this and other elements' zIndex
        if (parentWrapper.handleZ) { // this element or any of its siblings has a z index
            for (i = 0; i < childNodes.length; i++) {
                otherElement = childNodes[i];
                otherZIndex = attr(otherElement, 'zIndex');
                if (otherElement !== element && (
                        // insert before the first element with a higher zIndex
                        pInt(otherZIndex) > zIndex ||
                        // if no zIndex given, insert before the first element with a zIndex
                        (!defined(zIndex) && defined(otherZIndex))
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
        this.added = true;
        // fire an event for internal hooks
        fireEvent(this, 'add');
        return this;
    },
    /**
     * Removes a child either by removeChild or move to garbageBin.
     * Issue 490; in VML removeChild results in Orphaned nodes according to sIEve, discardElement does not.
     */
    safeRemoveChild: function (element) {
        let parentNode = element.parentNode;
        if (parentNode) {
            parentNode.removeChild(element);
        }
    },
    /**
     * Destroy the element and element wrapper
     */
    destroy: function () {
        let wrapper = this,
            element = wrapper.element || {},
            shadows = wrapper.shadows,
            key,
            i;
        // remove events
        element.onclick = element.onmouseout = element.onmouseover = element.onmousemove = null;
        stop(wrapper); // stop running animations
        if (wrapper.clipPath) {
            wrapper.clipPath = wrapper.clipPath.destroy();
        }
        // Destroy stops in case this is a gradient object
        if (wrapper.stops) {
            for (i = 0; i < wrapper.stops.length; i++) {
                wrapper.stops[i] = wrapper.stops[i].destroy();
            }
            wrapper.stops = null;
        }
        // remove element
        wrapper.safeRemoveChild(element);
        // destroy shadows
        if (shadows) {
            each(shadows, function (shadow) {
                wrapper.safeRemoveChild(shadow);
            });
        }
        // remove from alignObjects
        erase(wrapper.renderer.alignedObjects, wrapper);
        for (key in wrapper) {
            delete wrapper[key];
        }
        return null;
    },
    /**
     * Empty a group element
     */
    empty: function () {
        let element = this.element,
            childNodes = element.childNodes,
            i = childNodes.length;
        while (i--) {
            element.removeChild(childNodes[i]);
        }
    },
    /**
     * Add a shadow to the element. Must be done after the element is added to the DOM
     * @param {Boolean|Object} shadowOptions
     */
    shadow: function (shadowOptions, group, cutOff) {
        let shadows = [],
            i,
            shadow,
            element = this.element,
            strokeWidth,
            shadowWidth,
            shadowElementOpacity,
            transform;
        if (shadowOptions) {
            shadowWidth = pick(shadowOptions.width, 3);
            shadowElementOpacity = (shadowOptions.opacity || 0.15) / shadowWidth;
            transform =
                '(' + pick(shadowOptions.offsetX, 1) + ', ' + pick(shadowOptions.offsetY, 1) + ')';
            for (i = 1; i <= shadowWidth; i++) {
                shadow = element.cloneNode(0);
                strokeWidth = (shadowWidth * 2) + 1 - (2 * i);
                attr(shadow, {
                    'isShadow': 'true',
                    'stroke': shadowOptions.color || 'black',
                    'stroke-opacity': shadowElementOpacity * i,
                    'stroke-width': strokeWidth,
                    'transform': 'translate' + transform,
                    'fill': NONE
                });
                if (cutOff) {
                    attr(shadow, 'height', mathMax(attr(shadow, 'height') - strokeWidth, 0));
                    shadow.cutHeight = strokeWidth;
                }
                if (group) {
                    group.element.appendChild(shadow);
                } else {
                    element.parentNode.insertBefore(shadow, element);
                }
                shadows.push(shadow);
            }
            this.shadows = shadows;
        }
        return this;
    }
};
/**
 * The default SVG renderer
 */
let SVGRenderer = function () {
    this.init.apply(this, arguments);
};
SVGRenderer.prototype = {
    Element: SVGElement,
    /**
     * Initialize the SVGRenderer
     * @param {Object} container
     * @param {Number} width
     * @param {Number} height
     * @param {Boolean} forExport
     */
    init: function (container, width, height, forExport) {
        let renderer = this,
            loc = location,
            boxWrapper;
        boxWrapper = renderer.createElement('svg')
            .attr({
                xmlns: SVG_NS,
                version: '1.1'
            });
        container.appendChild(boxWrapper.element);
        // object properties
        renderer.isSVG = true;
        renderer.box = boxWrapper.element;
        renderer.boxWrapper = boxWrapper;
        renderer.alignedObjects = [];
        // Page url used for internal references. #24, #672, #1070
        renderer.url = (isFirefox || isWebKit) && doc.getElementsByTagName('base').length ?
            loc.href
                .replace(/#.*?$/, '') // remove the hash
                .replace(/([\('\)])/g, '\\$1') // escape parantheses and quotes
                .replace(/ /g, '%20') : // replace spaces (needed for Safari only)
            '';
        renderer.defs = this.createElement('defs').add();
        renderer.forExport = forExport;
        renderer.setSize(width, height, false);
    },
    /**
     * Detect whether the renderer is hidden. This happens when one of the parent elements
     * has display: none. #608.
     */
    isHidden: function () {
        return !this.boxWrapper.getBBox().width;
    },
    /**
     * Destroys the renderer and its allocated members.
     */
    destroy: function () {
        let renderer = this,
            rendererDefs = renderer.defs;
        renderer.box = null;
        renderer.boxWrapper = renderer.boxWrapper.destroy();
        // Defs are null in VMLRenderer
        // Otherwise, destroy them here.
        if (rendererDefs) {
            renderer.defs = rendererDefs.destroy();
        }
        renderer.alignedObjects = null;
        return null;
    },
    /**
     * Create a wrapper for an SVG element
     * @param {Object} nodeName
     */
    createElement: function (nodeName) {
        let wrapper = new this.Element();
        wrapper.init(this, nodeName);
        return wrapper;
    },
    /**
     * Dummy function for use in canvas renderer
     */
    draw: function () {},
    /**
     * Parse a simple HTML string into SVG tspans
     *
     * @param {Object} textNode The parent text SVG node
     */
    buildText: function (wrapper) {
        let textNode = wrapper.element,
            lines = pick(wrapper.textStr, '').toString()
                .replace(/<(b|strong)>/g, '<span style="font-weight:bold">')
                .replace(/<(i|em)>/g, '<span style="font-style:italic">')
                .replace(/<a/g, '<span')
                .replace(/<\/(b|strong|i|em|a)>/g, '</span>')
                .split(/<br.*?>/g),
            childNodes = textNode.childNodes,
            styleRegex = /style="([^"]+)"/,
            hrefRegex = /href="([^"]+)"/,
            parentX = attr(textNode, 'x'),
            textStyles = wrapper.styles,
            width = textStyles && textStyles.width && pInt(textStyles.width),
            textLineHeight = textStyles && textStyles.lineHeight,
            lastLine,
            GET_COMPUTED_STYLE = 'getComputedStyle',
            i = childNodes.length,
            linePositions = [];
        // Needed in IE9 because it doesn't report tspan's offsetHeight (#893)
        function getLineHeightByBBox(lineNo) {
            linePositions[lineNo] = textNode.getBBox ?
                textNode.getBBox().height :
                wrapper.renderer.fontMetrics(textNode.style.fontSize).h; // #990
            return mathRound(linePositions[lineNo] - (linePositions[lineNo - 1] || 0));
        }
        // remove old text
        while (i--) {
            textNode.removeChild(childNodes[i]);
        }
        if (width && !wrapper.added) {
            this.box.appendChild(textNode); // attach it to the DOM to read offset width
        }
        // remove empty line at end
        if (lines[lines.length - 1] === '') {
            lines.pop();
        }
        // build the lines
        each(lines, function (line, lineNo) {
            let spans, spanNo = 0, lineHeight;
            line = line.replace(/<span/g, '|||<span').replace(/<\/span>/g, '</span>|||');
            spans = line.split('|||');
            each(spans, function (span) {
                if (span !== '' || spans.length === 1) {
                    let attributes = {},
                        tspan = doc.createElementNS(SVG_NS, 'tspan'),
                        spanStyle; // #390
                    if (styleRegex.test(span)) {
                        spanStyle = span.match(styleRegex)[1].replace(/(;| |^)color([ :])/, '$1fill$2');
                        attr(tspan, 'style', spanStyle);
                    }
                    if (hrefRegex.test(span)) {
                        attr(tspan, 'onclick', 'location.href=\"' + span.match(hrefRegex)[1] + '\"');
                        css(tspan, { cursor: 'pointer' });
                    }
                    span = (span.replace(/<(.|\n)*?>/g, '') || ' ')
                        .replace(/&lt;/g, '<')
                        .replace(/&gt;/g, '>');
                    // issue #38 workaround.
                    /*if (reverse) {
                        arr = [];
                        i = span.length;
                        while (i--) {
                            arr.push(span.charAt(i));
                        }
                        span = arr.join('');
                    }*/
                    // add the text node
                    tspan.appendChild(doc.createTextNode(span));
                    if (!spanNo) { // first span in a line, align it to the left
                        attributes.x = parentX;
                    } else {
                        // Firefox ignores spaces at the front or end of the tspan
                        attributes.dx = 3; // space
                    }
                    // first span on subsequent line, add the line height
                    if (!spanNo) {
                        if (lineNo) {
                            // allow getting the right offset height in exporting in IE
                            if (!hasSVG && wrapper.renderer.forExport) {
                                css(tspan, { display: 'block' });
                            }
                            // Webkit and opera sometimes return 'normal' as the line height. In that
                            // case, webkit uses offsetHeight, while Opera falls back to 18
                            lineHeight = win[GET_COMPUTED_STYLE] &&
                                pInt(win[GET_COMPUTED_STYLE](lastLine, null).getPropertyValue('line-height'));
                            if (!lineHeight || isNaN(lineHeight)) {
                                lineHeight = textLineHeight || lastLine.offsetHeight || getLineHeightByBBox(lineNo) || 18;
                            }
                            attr(tspan, 'dy', lineHeight);
                        }
                        lastLine = tspan; // record for use in next line
                    }
                    // add attributes
                    attr(tspan, attributes);
                    // append it
                    textNode.appendChild(tspan);
                    spanNo++;
                    // check width and apply soft breaks
                    if (width) {
                        let words = span.replace(/([^\^])-/g, '$1- ').split(' '), // #1273
                            tooLong,
                            actualWidth,
                            rest = [];
                        while (words.length || rest.length) {
                            delete wrapper.bBox; // delete cache
                            actualWidth = wrapper.getBBox().width;
                            tooLong = actualWidth > width;
                            if (!tooLong || words.length === 1) { // new line needed
                                words = rest;
                                rest = [];
                                if (words.length) {
                                    tspan = doc.createElementNS(SVG_NS, 'tspan');
                                    attr(tspan, {
                                        dy: textLineHeight || 16,
                                        x: parentX
                                    });
                                    if (spanStyle) { // #390
                                        attr(tspan, 'style', spanStyle);
                                    }
                                    textNode.appendChild(tspan);
                                    if (actualWidth > width) { // a single word is pressing it out
                                        width = actualWidth;
                                    }
                                }
                            } else { // append to existing line tspan
                                tspan.removeChild(tspan.firstChild);
                                rest.unshift(words.pop());
                            }
                            if (words.length) {
                                tspan.appendChild(doc.createTextNode(words.join(' ').replace(/- /g, '-')));
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
            // Substract due to #1129. Now bottom and left axis gridlines behave the same.
            points[1] = points[4] = mathRound(points[1]) - (width % 2 / 2);
        }
        if (points[2] === points[5]) {
            points[2] = points[5] = mathRound(points[2]) + (width % 2 / 2);
        }
        return points;
    },
    /**
     * Draw a path
     * @param {Array} path An SVG path in array form
     */
    path: function (path) {
        let attr = {
            fill: NONE
        };
        if (isArray(path)) {
            attr.d = path;
        } else if (isObject(path)) { // attributes
            extend(attr, path);
        }
        return this.createElement('path').attr(attr);
    },
    /**
     * Draw and return a rectangle
     * @param {Number} x Left position
     * @param {Number} y Top position
     * @param {Number} width
     * @param {Number} height
     * @param {Number} r Border corner radius
     * @param {Number} strokeWidth A stroke width can be supplied to allow crisp drawing
     */
    rect: function (x, y, width, height, r, strokeWidth) {
        r = isObject(x) ? x.r : r;
        let wrapper = this.createElement('rect').attr({
                rx: r,
                ry: r,
                fill: NONE
            });
        return wrapper.attr(
                isObject(x) ?
                    x :
                    // do not crispify when an object is passed in (as in column charts)
                    wrapper.crisp(strokeWidth, x, y, mathMax(width, 0), mathMax(height, 0))
            );
    },
    /**
     * Resize the box and re-align all aligned elements
     * @param {Object} width
     * @param {Object} height
     * @param {Boolean} animate
     *
     */
    setSize: function (width, height, animate) {
        let renderer = this,
            alignedObjects = renderer.alignedObjects,
            i = alignedObjects.length;
        renderer.width = width;
        renderer.height = height;
        renderer.boxWrapper[pick(animate, true) ? 'animate' : 'attr']({
            width: width,
            height: height
        });
        while (i--) {
            alignedObjects[i].align();
        }
    },
    /**
     * Create a group
     * @param {String} name The group will be given a class name of 'highcharts-{name}'.
     *     This can be used for styling and scripting.
     */
    g: function (name) {
        let elem = this.createElement('g');
        return defined(name) ? elem.attr({ 'class': PREFIX + name }) : elem;
    },
    /**
     * Draw a symbol out of pre-defined shape paths from the namespace 'symbol' object.
     *
     * @param {Object} symbol
     * @param {Object} x
     * @param {Object} y
     * @param {Object} radius
     * @param {Object} options
     */
    symbol: function (symbol, x, y, width, height, options) {
        let obj,
            // get the symbol definition function
            symbolFn = this.symbols[symbol],
            // check if there's a path defined for this symbol
            path = symbolFn && symbolFn(
                mathRound(x),
                mathRound(y),
                width,
                height,
                options
            ),
            imageElement,
            imageRegex = /^url\((.*?)\)$/,
            imageSrc,
            imageSize,
            centerImage;
        if (path) {
            obj = this.path(path);
            // expando properties for use in animate and attr
            extend(obj, {
                symbolName: symbol,
                x: x,
                y: y,
                width: width,
                height: height
            });
            if (options) {
                extend(obj, options);
            }
        }
        return obj;
    },
    /**
     * An extendable collection of functions for defining symbol paths.
     */
    symbols: {
        'circle': function (x, y, w, h) {
            let cpw = 0.166 * w;
            return [
                M, x + w / 2, y,
                'C', x + w + cpw, y, x + w + cpw, y + h, x + w / 2, y + h,
                'C', x - cpw, y + h, x - cpw, y, x + w / 2, y,
                'Z'
            ];
        },
        'square': function (x, y, w, h) {
            return [
                M, x, y,
                L, x + w, y,
                x + w, y + h,
                x, y + h,
                'Z'
            ];
        },
        'triangle': function (x, y, w, h) {
            return [
                M, x + w / 2, y,
                L, x + w, y + h,
                x, y + h,
                'Z'
            ];
        },
        'triangle-down': function (x, y, w, h) {
            return [
                M, x, y,
                L, x + w, y,
                x + w / 2, y + h,
                'Z'
            ];
        },
        'diamond': function (x, y, w, h) {
            return [
                M, x + w / 2, y,
                L, x + w, y + h / 2,
                x + w / 2, y + h,
                x, y + h / 2,
                'Z'
            ];
        }
    },
    /**
     * Define a clipping rectangle
     * @param {String} id
     * @param {Number} x
     * @param {Number} y
     * @param {Number} width
     * @param {Number} height
     */
    clipRect: function (x, y, width, height) {
        let wrapper,
            id = PREFIX + idCounter++,
            clipPath = this.createElement('clipPath').attr({
                id: id
            }).add(this.defs);
        wrapper = this.rect(x, y, width, height, 0).add(clipPath);
        wrapper.id = id;
        wrapper.clipPath = clipPath;
        return wrapper;
    },
    /**
     * Take a color and return it if it's a string, make it a gradient if it's a
     * gradient configuration object. Prior to Highstock, an array was used to define
     * a linear gradient with pixel positions relative to the SVG. In newer versions
     * we change the coordinates to apply relative to the shape, using coordinates
     * 0-1 within the shape. To preserve backwards compatibility, linearGradient
     * in this definition is an object of x1, y1, x2 and y2.
     *
     * @param {Object} color The color or config object
     */
    color: function (color, elem, prop) {
        let renderer = this,
            colorObject,
            regexRgba = /^rgba/,
            gradName,
            gradAttr,
            stops,
            stopColor,
            stopOpacity,
            radialReference,
            n,
            id,
            key = [];
        // Webkit and Batik can't show rgba.
        if (regexRgba.test(color)) {
            colorObject = Color(color);
            attr(elem, prop + '-opacity', colorObject.get('a'));
            return colorObject.get('rgb');
        } else {
            // Remove the opacity attribute added above. Does not throw if the attribute is not there.
            elem.removeAttribute(prop + '-opacity');
            return color;
        }
    },
    /**
     * Add text to the SVG object
     * @param {String} str
     * @param {Number} x Left position
     * @param {Number} y Top position
     * @param {Boolean} useHTML Use HTML to render the text
     */
    text: function (str, x, y, useHTML) {
        // declare variables
        let renderer = this,
            wrapper;
        x = mathRound(pick(x, 0));
        y = mathRound(pick(y, 0));
        wrapper = renderer.createElement('text')
            .attr({
                x: x,
                y: y,
                text: str
            });
        wrapper.x = x;
        wrapper.y = y;
        return wrapper;
    },
    /**
     * Utility to return the baseline offset and total line height from the font size
     */
    fontMetrics: function (fontSize) {
        fontSize = pInt(fontSize || 11);
        // Empirical values found by comparing font size and bounding box height.
        // Applies to the default font family. http://jsfiddle.net/highcharts/7xvn7/
        let lineHeight = fontSize < 24 ? fontSize + 4 : mathRound(fontSize * 1.2),
            baseline = mathRound(lineHeight * 0.8);
        return {
            h: lineHeight,
            b: baseline
        };
    },
    /**
     * Add a label, a text item that can hold a colored or gradient background
     * as well as a border and shadow.
     * @param {string} str
     * @param {Number} x
     * @param {Number} y
     * @param {String} shape
     * @param {Number} anchorX In case the shape has a pointer, like a flag, this is the
     *    coordinates it should be pinned to
     * @param {Number} anchorY
     * @param {Boolean} baseline Whether to position the label relative to the text baseline,
     *    like renderer.text, or to the upper border of the rectangle.
     * @param {String} className Class name for the group
     */
    label: function (str, x, y, shape, anchorX, anchorY, useHTML, baseline, className) {
        let renderer = this,
            wrapper = renderer.g(className),
            text = renderer.text('', 0, 0, useHTML)
                .attr({
                    zIndex: 1
                }),
                //.add(wrapper),
            box,
            bBox,
            alignFactor = 0,
            padding = 3,
            width,
            height,
            wrapperX,
            wrapperY,
            crispAdjust = 0,
            deferredAttr = {},
            baselineOffset,
            attrSetters = wrapper.attrSetters,
            needsBox;
        /**
         * This function runs after the label is added to the DOM (when the bounding box is
         * available), and after the text of the label is updated to detect the new bounding
         * box and reflect it in the border box.
         */
        function updateBoxSize() {
            let boxY,
                style = text.element.style;
            bBox = (width === undefined || height === undefined || wrapper.styles.textAlign) &&
                text.getBBox();
            wrapper.width = (width || bBox.width || 0) + 2 * padding;
            wrapper.height = (height || bBox.height || 0) + 2 * padding;
            // update the label-scoped y offset
            baselineOffset = padding + renderer.fontMetrics(style && style.fontSize).b;
            if (needsBox) {
                // create the border box if it is not already present
                if (!box) {
                    boxY = baseline ? -baselineOffset : 0;
                    wrapper.box = box = shape ?
                        renderer.symbol(shape, -alignFactor * padding, boxY, wrapper.width, wrapper.height) :
                        renderer.rect(-alignFactor * padding, boxY, wrapper.width, wrapper.height, 0, deferredAttr[STROKE_WIDTH]);
                    box.add(wrapper);
                }
                // apply the box attributes
                box.attr(merge({
                    width: wrapper.width,
                    height: wrapper.height
                }, deferredAttr));
                deferredAttr = null;
            }
        }
        /**
         * This function runs after setting text or padding, but only if padding is changed
         */
        function updateTextPadding() {
            let styles = wrapper.styles,
                textAlign = styles && styles.textAlign,
                x = padding * (1 - alignFactor),
                y;
            // determin y based on the baseline
            y = baseline ? 0 : baselineOffset;
            // compensate for alignment
            if (defined(width) && (textAlign === 'center' || textAlign === 'right')) {
                x += { center: 0.5, right: 1 }[textAlign] * (width - bBox.width);
            }
            // update if anything changed
            if (x !== text.x || y !== text.y) {
                text.attr({
                    x: x,
                    y: y
                });
            }
            // record current values
            text.x = x;
            text.y = y;
        }
        /**
         * Set a box attribute, or defer it if the box is not yet created
         * @param {Object} key
         * @param {Object} value
         */
        function boxAttr(key, value) {
            if (box) {
                box.attr(key, value);
            } else {
                deferredAttr[key] = value;
            }
        }
        function getSizeAfterAdd() {
            text.add(wrapper);
            wrapper.attr({
                text: str, // alignment is available now
                x: x,
                y: y
            });
            if (box && defined(anchorX)) {
                wrapper.attr({
                    anchorX: anchorX,
                    anchorY: anchorY
                });
            }
        }
        /**
         * After the text element is added, get the desired size of the border box
         * and add it before the text in the DOM.
         */
        addEvent(wrapper, 'add', getSizeAfterAdd);
        /*
         * Add specific attribute setters.
         */
        // only change local variables
        attrSetters.width = function (value) {
            width = value;
            return false;
        };
        attrSetters.height = function (value) {
            height = value;
            return false;
        };
        attrSetters.padding = function (value) {
            if (defined(value) && value !== padding) {
                padding = value;
                updateTextPadding();
            }
            return false;
        };
        // change local variable and set attribue as well
        attrSetters.align = function (value) {
            alignFactor = { left: 0, center: 0.5, right: 1 }[value];
            return false; // prevent setting text-anchor on the group
        };
        // apply these to the box and the text alike
        attrSetters.text = function (value, key) {
            text.attr(key, value);
            updateBoxSize();
            updateTextPadding();
            return false;
        };
        // apply these to the box but not to the text
        attrSetters[STROKE_WIDTH] = function (value, key) {
            needsBox = true;
            crispAdjust = value % 2 / 2;
            boxAttr(key, value);
            return false;
        };
        attrSetters.stroke = attrSetters.fill = attrSetters.r = function (value, key) {
            if (key === 'fill') {
                needsBox = true;
            }
            boxAttr(key, value);
            return false;
        };
        attrSetters.anchorX = function (value, key) {
            anchorX = value;
            boxAttr(key, value + crispAdjust - wrapperX);
            return false;
        };
        attrSetters.anchorY = function (value, key) {
            anchorY = value;
            boxAttr(key, value - wrapperY);
            return false;
        };
        // rename attributes
        attrSetters.x = function (value) {
            wrapper.x = value; // for animation getter
            value -= alignFactor * ((width || bBox.width) + padding);
            wrapperX = mathRound(value);
            wrapper.attr('translateX', wrapperX);
            return false;
        };
        attrSetters.y = function (value) {
            wrapperY = wrapper.y = mathRound(value);
            wrapper.attr('translateY', value);
            return false;
        };
        // Redirect certain methods to either the box or the text
        let baseCss = wrapper.css;
        return extend(wrapper, {
            /**
             * Pick up some properties and apply them to the text instead of the wrapper
             */
            css: function (styles) {
                if (styles) {
                    let textStyles = {};
                    styles = merge({}, styles); // create a copy to avoid altering the original object (#537)
                    each(['fontSize', 'fontWeight', 'color', 'lineHeight', 'width'], function (prop) {
                        if (styles[prop] !== UNDEFINED) {
                            textStyles[prop] = styles[prop];
                            delete styles[prop];
                        }
                    });
                    text.css(textStyles);
                }
                return baseCss.call(wrapper, styles);
            },
            /**
             * Return the bounding box of the box, not the group
             */
            getBBox: function () {
                return {
                    width: bBox.width + 2 * padding,
                    height: bBox.height + 2 * padding,
                    x: bBox.x - padding,
                    y: bBox.y - padding
                };
            },
            /**
             * Apply the shadow to the box
             */
            shadow: function (b) {
                if (box) {
                    box.shadow(b);
                }
                return wrapper;
            },
            /**
             * Destroy and release memory.
             */
            destroy: function () {
                removeEvent(wrapper, 'add', getSizeAfterAdd);
                // Added by button implementation
                removeEvent(wrapper.element, 'mouseenter');
                removeEvent(wrapper.element, 'mouseleave');
                if (text) {
                    text = text.destroy();
                }
                if (box) {
                    box = box.destroy();
                }
                // Call base implementation to destroy the rest
                SVGElement.prototype.destroy.call(wrapper);
                // Release local pointers (#1298)
                wrapper = renderer = updateBoxSize = updateTextPadding = boxAttr = getSizeAfterAdd = null;
            }
        });
    }
}; // end SVGRenderer
/**
 * The Tick class
 */
function Tick(axis, pos, type) {
    this.axis = axis;
    this.pos = pos;
    this.type = type || '';
    this.isNew = true;
    if (!type) {
        this.addLabel();
    }
}
Tick.prototype = {
    /**
     * Write the tick label
     */
    addLabel: function () {
        let tick = this,
            axis = tick.axis,
            options = axis.options,
            chart = axis.chart,
            horiz = axis.horiz,
            categories = axis.categories,
            pos = tick.pos,
            labelOptions = options.labels,
            str,
            tickPositions = axis.tickPositions,
            width = (categories && horiz && categories.length &&
                !labelOptions.step && !labelOptions.staggerLines &&
                !labelOptions.rotation &&
                chart.plotWidth / tickPositions.length) ||
                (!horiz && chart.plotWidth / 2),
            isFirst = pos === tickPositions[0],
            isLast = pos === tickPositions[tickPositions.length - 1],
            css,
            attr,
            value = categories && defined(categories[pos]) ? categories[pos] : pos,
            label = tick.label,
            tickPositionInfo = tickPositions.info,
            dateTimeLabelFormat;
        if (categories && !defined(categories[pos])) {
            return;
        }
        // Set the datetime label format. If a higher rank is set for this position, use that. If not,
        // use the general format.
        if (axis.isDatetimeAxis && tickPositionInfo) {
            dateTimeLabelFormat = options.dateTimeLabelFormats[tickPositionInfo.higherRanks[pos] || tickPositionInfo.unitName];
        }
        // set properties for access in render method
        tick.isFirst = isFirst;
        tick.isLast = isLast;
        // get the string
        str = axis.labelFormatter.call({
            axis: axis,
            chart: chart,
            isFirst: isFirst,
            isLast: isLast,
            dateTimeLabelFormat: dateTimeLabelFormat,
            value: axis.isLog ? correctFloat(lin2log(value)) : value
        });
        // prepare CSS
        css = width && { width: mathMax(1, mathRound(width - 2 * (labelOptions.padding || 10))) + 'px' };
        css = extend(css, labelOptions.style);
        // first call
        if (!defined(label)) {
            attr = {
                align: labelOptions.align
            };
            if (isNumber(labelOptions.rotation)) {
                attr.rotation = labelOptions.rotation;
            }
            tick.label =
                defined(str) && labelOptions.enabled ?
                    chart.renderer.text(
                            str,
                            0,
                            0,
                            labelOptions.useHTML
                        )
                        .attr(attr)
                        // without position absolute, IE export sometimes is wrong
                        .css(css)
                        .add(axis.labelGroup) :
                    null;
        // update
        } else if (label) {
            label.attr({
                    text: str
                })
                .css(css);
        }
    },
    /**
     * Get the offset height or width of the label
     */
    getLabelSize: function () {
        let label = this.label,
            axis = this.axis;
        return label ?
            ((this.labelBBox = label.getBBox()))[axis.horiz ? 'height' : 'width'] :
            0;
    },
    /**
     * Find how far the labels extend to the right and left of the tick's x position. Used for anti-collision
     * detection with overflow logic.
     */
    getLabelSides: function () {
        let bBox = this.labelBBox, // assume getLabelSize has run at this point
            axis = this.axis,
            options = axis.options,
            labelOptions = options.labels,
            width = bBox.width,
            leftSide = width * { left: 0, center: 0.5, right: 1 }[labelOptions.align] - labelOptions.x;
        return [-leftSide, width - leftSide];
    },
    /**
     * Handle the label overflow by adjusting the labels to the left and right edge, or
     * hide them if they collide into the neighbour label.
     */
    handleOverflow: function (index, xy) {
        let show = true,
            axis = this.axis,
            chart = axis.chart,
            isFirst = this.isFirst,
            isLast = this.isLast,
            x = xy.x,
            reversed = axis.reversed,
            tickPositions = axis.tickPositions;
        if (isFirst || isLast) {
            let sides = this.getLabelSides(),
                leftSide = sides[0],
                rightSide = sides[1],
                plotLeft = chart.plotLeft,
                plotRight = plotLeft + axis.len,
                neighbour = axis.ticks[tickPositions[index + (isFirst ? 1 : -1)]],
                neighbourEdge = neighbour && neighbour.label.xy && neighbour.label.xy.x + neighbour.getLabelSides()[isFirst ? 0 : 1];
            if ((isFirst && !reversed) || (isLast && reversed)) {
                // Is the label spilling out to the left of the plot area?
                if (x + leftSide < plotLeft) {
                    // Align it to plot left
                    x = plotLeft - leftSide;
                    // Hide it if it now overlaps the neighbour label
                    if (neighbour && x + rightSide > neighbourEdge) {
                        show = false;
                    }
                }
            } else {
                // Is the label spilling out to the right of the plot area?
                if (x + rightSide > plotRight) {
                    // Align it to plot right
                    x = plotRight - rightSide;
                    // Hide it if it now overlaps the neighbour label
                    if (neighbour && x + leftSide < neighbourEdge) {
                        show = false;
                    }
                }
            }
            // Set the modified x position of the label
            xy.x = x;
        }
        return show;
    },
    /**
     * Get the x and y position for ticks and labels
     */
    getPosition: function (horiz, pos, tickmarkOffset, old) {
        let axis = this.axis,
            chart = axis.chart,
            cHeight = (old && chart.oldChartHeight) || chart.chartHeight;
        return {
            x: horiz ?
                axis.translate(pos + tickmarkOffset, null, null, old) + axis.transB :
                axis.left + axis.offset + (axis.opposite ? ((old && chart.oldChartWidth) || chart.chartWidth) - axis.right - axis.left : 0),
            y: horiz ?
                cHeight - axis.bottom + axis.offset - (axis.opposite ? axis.height : 0) :
                cHeight - axis.translate(pos + tickmarkOffset, null, null, old) - axis.transB
        };
    },
    /**
     * Get the x, y position of the tick label
     */
    getLabelPosition: function (x, y, label, horiz, labelOptions, tickmarkOffset, index, step) {
        let axis = this.axis,
            transA = axis.transA,
            reversed = axis.reversed,
            staggerLines = axis.staggerLines;
        x = x + labelOptions.x - (tickmarkOffset && horiz ?
            tickmarkOffset * transA * (reversed ? -1 : 1) : 0);
        y = y + labelOptions.y - (tickmarkOffset && !horiz ?
            tickmarkOffset * transA * (reversed ? 1 : -1) : 0);
        // Vertically centered
        if (!defined(labelOptions.y)) {
            y += pInt(label.styles.lineHeight) * 0.9 - label.getBBox().height / 2;
        }
        // Correct for staggered labels
        if (staggerLines) {
            y += (index / (step || 1) % staggerLines) * 16;
        }
        return {
            x: x,
            y: y
        };
    },
    /**
     * Extendible method to return the path of the marker
     */
    getMarkPath: function (x, y, tickLength, tickWidth, horiz, renderer) {
        return renderer.crispLine([
                M,
                x,
                y,
                L,
                x + (horiz ? 0 : -tickLength),
                y + (horiz ? tickLength : 0)
            ], tickWidth);
    },
    /**
     * Put everything in place
     *
     * @param index {Number}
     * @param old {Boolean} Use old coordinates to prepare an animation into new position
     */
    render: function (index, old) {
        let tick = this,
            axis = tick.axis,
            options = axis.options,
            chart = axis.chart,
            renderer = chart.renderer,
            horiz = axis.horiz,
            type = tick.type,
            label = tick.label,
            pos = tick.pos,
            labelOptions = options.labels,
            gridLine = tick.gridLine,
            gridPrefix = type ? type + 'Grid' : 'grid',
            tickPrefix = type ? type + 'Tick' : 'tick',
            gridLineWidth = options[gridPrefix + 'LineWidth'],
            gridLineColor = options[gridPrefix + 'LineColor'],
            dashStyle = options[gridPrefix + 'LineDashStyle'],
            tickLength = options[tickPrefix + 'Length'],
            tickWidth = options[tickPrefix + 'Width'] || 0,
            tickColor = options[tickPrefix + 'Color'],
            tickPosition = options[tickPrefix + 'Position'],
            gridLinePath,
            mark = tick.mark,
            markPath,
            step = labelOptions.step,
            attribs,
            show = true,
            tickmarkOffset = axis.tickmarkOffset,
            xy = tick.getPosition(horiz, pos, tickmarkOffset, old),
            x = xy.x,
            y = xy.y,
            staggerLines = axis.staggerLines;
        // create the grid line
        if (gridLineWidth) {
            gridLinePath = axis.getPlotLinePath(pos + tickmarkOffset, gridLineWidth, old);
            if (gridLine === UNDEFINED) {
                attribs = {
                    stroke: gridLineColor,
                    'stroke-width': gridLineWidth
                };
                if (dashStyle) {
                    attribs.dashstyle = dashStyle;
                }
                if (!type) {
                    attribs.zIndex = 1;
                }
                tick.gridLine = gridLine =
                    gridLineWidth ?
                        renderer.path(gridLinePath)
                            .attr(attribs).add(axis.gridGroup) :
                        null;
            }
            // If the parameter 'old' is set, the current call will be followed
            // by another call, therefore do not do any animations this time
            if (!old && gridLine && gridLinePath) {
                gridLine[tick.isNew ? 'attr' : 'animate']({
                    d: gridLinePath
                });
            }
        }
        // create the tick mark
        if (tickWidth && tickLength) {
            // negate the length
            if (tickPosition === 'inside') {
                tickLength = -tickLength;
            }
            if (axis.opposite) {
                tickLength = -tickLength;
            }
            markPath = tick.getMarkPath(x, y, tickLength, tickWidth, horiz, renderer);
            if (mark) { // updating
                mark.animate({
                    d: markPath
                });
            } else { // first time
                tick.mark = renderer.path(
                    markPath
                ).attr({
                    stroke: tickColor,
                    'stroke-width': tickWidth
                }).add(axis.axisGroup);
            }
        }
        // the label is created on init - now move it into place
        if (label && !isNaN(x)) {
            label.xy = xy = tick.getLabelPosition(x, y, label, horiz, labelOptions, tickmarkOffset, index, step);
            // apply show first and show last
            if ((tick.isFirst && !pick(options.showFirstLabel, 1)) ||
                    (tick.isLast && !pick(options.showLastLabel, 1))) {
                show = false;
            // Handle label overflow and show or hide accordingly
            } else if (!staggerLines && horiz && labelOptions.overflow === 'justify' && !tick.handleOverflow(index, xy)) {
                show = false;
            }
            // apply step
            if (step && index % step) {
                // show those indices dividable by step
                show = false;
            }
            // Set the new position, and show or hide
            if (show) {
                label[tick.isNew ? 'attr' : 'animate'](xy);
                tick.isNew = false;
            } else {
                label.attr('y', -9999); // #1338
            }
        }
    },
    /**
     * Destructor for the tick prototype
     */
    destroy: function () {
        destroyObjectProperties(this, this.axis);
    }
};
/**
 * The object wrapper for plot lines and plot bands
 * @param {Object} options
 */
function PlotLineOrBand(axis, options) {
    this.axis = axis;
    if (options) {
        this.options = options;
        this.id = options.id;
    }
    //plotLine.render()
    return this;
}
PlotLineOrBand.prototype = {
    /**
     * Render the plot line or plot band. If it is already existing,
     * move it.
     */
    render: function () {
        let plotLine = this,
            axis = plotLine.axis,
            horiz = axis.horiz,
            halfPointRange = (axis.pointRange || 0) / 2,
            options = plotLine.options,
            optionsLabel = options.label,
            label = plotLine.label,
            width = options.width,
            to = options.to,
            from = options.from,
            isBand = defined(from) && defined(to),
            value = options.value,
            dashStyle = options.dashStyle,
            svgElem = plotLine.svgElem,
            path = [],
            addEvent,
            eventType,
            xs,
            ys,
            x,
            y,
            color = options.color,
            zIndex = options.zIndex,
            events = options.events,
            attribs,
            renderer = axis.chart.renderer;
        // logarithmic conversion
        if (axis.isLog) {
            from = log2lin(from);
            to = log2lin(to);
            value = log2lin(value);
        }
        // plot line
        if (width) {
            path = axis.getPlotLinePath(value, width);
            attribs = {
                stroke: color,
                'stroke-width': width
            };
            if (dashStyle) {
                attribs.dashstyle = dashStyle;
            }
        } else if (isBand) { // plot band
            // keep within plot area
            from = mathMax(from, axis.min - halfPointRange);
            to = mathMin(to, axis.max + halfPointRange);
            path = axis.getPlotBandPath(from, to, options);
            attribs = {
                fill: color
            };
            if (options.borderWidth) {
                attribs.stroke = options.borderColor;
                attribs['stroke-width'] = options.borderWidth;
            }
        } else {
            return;
        }
        // zIndex
        if (defined(zIndex)) {
            attribs.zIndex = zIndex;
        }
        // common for lines and bands
        if (svgElem) {
            if (path) {
                svgElem.animate({
                    d: path
                }, null, svgElem.onGetPath);
            } else {
                svgElem.hide();
                svgElem.onGetPath = function () {
                    svgElem.show();
                };
            }
        } else if (path && path.length) {
            plotLine.svgElem = svgElem = renderer.path(path)
                .attr(attribs).add();
            // events
            if (events) {
                addEvent = function (eventType) {
                    svgElem.on(eventType, function (e) {
                        events[eventType].apply(plotLine, [e]);
                    });
                };
                for (eventType in events) {
                    addEvent(eventType);
                }
            }
        }
        // the plot band/line label
        if (optionsLabel && defined(optionsLabel.text) && path && path.length && axis.width > 0 && axis.height > 0) {
            // apply defaults
            optionsLabel = merge({
                align: horiz && isBand && 'center',
                x: horiz ? !isBand && 4 : 10,
                verticalAlign : !horiz && isBand && 'middle',
                y: horiz ? isBand ? 16 : 10 : isBand ? 6 : -4,
                rotation: horiz && !isBand && 90
            }, optionsLabel);
            // add the SVG element
            if (!label) {
                plotLine.label = label = renderer.text(
                        optionsLabel.text,
                        0,
                        0
                    )
                    .attr({
                        align: optionsLabel.textAlign || optionsLabel.align,
                        rotation: optionsLabel.rotation,
                        zIndex: zIndex
                    })
                    .css(optionsLabel.style)
                    .add();
            }
            // get the bounding box and align the label
            xs = [path[1], path[4], pick(path[6], path[1])];
            ys = [path[2], path[5], pick(path[7], path[2])];
            x = arrayMin(xs);
            y = arrayMin(ys);
            label.align(optionsLabel, false, {
                x: x,
                y: y,
                width: arrayMax(xs) - x,
                height: arrayMax(ys) - y
            });
            label.show();
        } else if (label) { // move out of sight
            label.hide();
        }
        // chainable
        return plotLine;
    },
    /**
     * Remove the plot line or band
     */
    destroy: function () {
        let plotLine = this,
            axis = plotLine.axis;
        // remove it from the lookup
        erase(axis.plotLinesAndBands, plotLine);
        destroyObjectProperties(plotLine, this.axis);
    }
};
/**
 * Create a new axis object
 * @param {Object} chart
 * @param {Object} options
 */
function Axis() {
    this.init.apply(this, arguments);
}
Axis.prototype = {
    /**
     * Default options for the X axis - the Y axis has extended defaults
     */
    defaultOptions: {
        // allowDecimals: null,
        // categories: [],
        dateTimeLabelFormats: {
            millisecond: '%H:%M:%S.%L',
            second: '%H:%M:%S',
            minute: '%H:%M',
            hour: '%H:%M',
            day: '%e. %b',
            week: '%e. %b',
            month: '%b \'%y',
            year: '%Y'
        },
        endOnTick: false,
        gridLineColor: '#C0C0C0',
        // gridLineDashStyle: 'solid',
        // gridLineWidth: 0,
        // reversed: false,
        labels: defaultLabelOptions,
            // { step: null },
        lineColor: '#C0D0E0',
        lineWidth: 1,
        //max: undefined,
        //min: undefined,
        minPadding: 0.01,
        maxPadding: 0.01,
        //minRange: null,
        minorGridLineColor: '#E0E0E0',
        // minorGridLineDashStyle: null,
        minorGridLineWidth: 1,
        minorTickColor: '#A0A0A0',
        //minorTickInterval: null,
        minorTickLength: 2,
        minorTickPosition: 'outside', // inside or outside
        //minorTickWidth: 0,
        //opposite: false,
        //offset: 0,
        //plotBands: [{
        //    events: {},
        //    zIndex: 1,
        //    labels: { align, x, verticalAlign, y, style, rotation, textAlign }
        //}],
        //plotLines: [{
        //    events: {}
        //  dashStyle: {}
        //    zIndex:
        //    labels: { align, x, verticalAlign, y, style, rotation, textAlign }
        //}],
        //reversed: false,
        // showFirstLabel: true,
        // showLastLabel: true,
        startOfWeek: 1,
        startOnTick: false,
        tickColor: '#C0D0E0',
        //tickInterval: null,
        tickLength: 5,
        tickmarkPlacement: 'between', // on or between
        tickPixelInterval: 100,
        tickPosition: 'outside',
        tickWidth: 1,
        type: 'linear' // linear, logarithmic or datetime
    },
    /**
     * This options set extends the defaultOptions for Y axes
     */
    defaultYAxisOptions: {
        endOnTick: true,
        gridLineWidth: 1,
        tickPixelInterval: 72,
        showLastLabel: true,
        labels: {
            align: 'right',
            x: -8,
            y: 3
        },
        lineWidth: 0,
        maxPadding: 0.05,
        minPadding: 0.05,
        startOnTick: true,
        tickWidth: 0
    },
    /**
     * These options extend the defaultOptions for left axes
     */
    defaultLeftAxisOptions: {
        labels: {
            align: 'right',
            x: -8,
            y: null
        }
    },
    /**
     * These options extend the defaultOptions for right axes
     */
    defaultRightAxisOptions: {
        labels: {
            align: 'left',
            x: 8,
            y: null
        }
    },
    /**
     * These options extend the defaultOptions for bottom axes
     */
    defaultBottomAxisOptions: {
        labels: {
            align: 'center',
            x: 0,
            y: 14
            // overflow: undefined,
            // staggerLines: null
        }
    },
    /**
     * These options extend the defaultOptions for left axes
     */
    defaultTopAxisOptions: {
        labels: {
            align: 'center',
            x: 0,
            y: -5
            // overflow: undefined
            // staggerLines: null
        }
    },
    /**
     * Initialize the axis
     */
    init: function (chart, userOptions) {
        let isXAxis = userOptions.isX,
            axis = this;
        // Flag, is the axis horizontal
        axis.horiz = isXAxis;
        // Flag, isXAxis
        axis.isXAxis = isXAxis;
        axis.xOrY = isXAxis ? 'x' : 'y';
        axis.opposite = userOptions.opposite; // needed in setOptions
        axis.side = axis.horiz ?
                (axis.opposite ? 0 : 2) : // top : bottom
                (axis.opposite ? 1 : 3);  // right : left
        axis.setOptions(userOptions);
        let options = this.options,
            type = options.type,
            isDatetimeAxis = type === 'datetime';
        axis.labelFormatter = options.labels.formatter || axis.defaultLabelFormatter; // can be overwritten by dynamic format
        // Flag, stagger lines or not
        axis.staggerLines = axis.horiz && options.labels.staggerLines;
        axis.userOptions = userOptions;
        axis.minPixelPadding = 0;
        //axis.ignoreMinPadding = UNDEFINED; // can be set to true by a column or bar series
        //axis.ignoreMaxPadding = UNDEFINED;
        axis.chart = chart;
        axis.reversed = options.reversed;
        // Initial categories
        axis.categories = options.categories;
        // Elements
        //axis.axisGroup = UNDEFINED;
        //axis.gridGroup = UNDEFINED;
        //axis.axisLine = UNDEFINED;
        // Flag if type === logarithmic
        axis.isLog = type === 'logarithmic';
        // Flag if type === datetime
        axis.isDatetimeAxis = isDatetimeAxis;
        // Flag if percentage mode
        // Tick positions
        //axis.tickPositions = UNDEFINED; // array containing predefined positions
        // Tick intervals
        //axis.tickInterval = UNDEFINED;
        //axis.minorTickInterval = UNDEFINED;
        axis.tickmarkOffset = (options.categories && options.tickmarkPlacement === 'between') ? 0.5 : 0;
        // Major ticks
        axis.ticks = {};
        // Minor ticks
        axis.minorTicks = {};
        //axis.tickAmount = UNDEFINED;
        // List of plotLines/Bands
        axis.plotLinesAndBands = [];
        // Alternate bands
        axis.alternateBands = {};
        // Axis metrics
        //axis.left = UNDEFINED;
        //axis.top = UNDEFINED;
        //axis.width = UNDEFINED;
        //axis.height = UNDEFINED;
        //axis.bottom = UNDEFINED;
        //axis.right = UNDEFINED;
        //axis.transA = UNDEFINED;
        //axis.transB = UNDEFINED;
        //axis.oldTransA = UNDEFINED;
        axis.len = 0;
        //axis.oldMin = UNDEFINED;
        //axis.oldMax = UNDEFINED;
        //axis.oldUserMin = UNDEFINED;
        //axis.oldUserMax = UNDEFINED;
        //axis.oldAxisLength = UNDEFINED;
        axis.minRange = axis.userMinRange = options.minRange || options.maxZoom;
        axis.range = options.range;
        axis.offset = options.offset || 0;
        // Min and max in the data
        //axis.dataMin = UNDEFINED,
        //axis.dataMax = UNDEFINED,
        // The axis range
        axis.max = null;
        axis.min = null;
        // User set min and max
        //axis.userMin = UNDEFINED,
        //axis.userMax = UNDEFINED,
        // Run Axis
        let eventType,
            events = axis.options.events;
        // Register
        chart.axisList.push(axis);
        chart[isXAxis ? 'xAxis' : 'yAxis'].push(axis);
        axis.seriesList = []; // populated by Series
        axis.removePlotBand = axis.removePlotBandOrLine;
        axis.removePlotLine = axis.removePlotBandOrLine;
        axis.addPlotBand = axis.addPlotBandOrLine;
        axis.addPlotLine = axis.addPlotBandOrLine;
        // register event listeners
        for (eventType in events) {
            addEvent(axis, eventType, events[eventType]);
        }
        // extend logarithmic axis
        if (axis.isLog) {
            axis.val2lin = log2lin;
            axis.lin2val = lin2log;
        }
    },
    /**
     * Merge and set options
     */
    setOptions: function (userOptions) {
        this.options = merge(
            this.defaultOptions,
            this.isXAxis ? {} : this.defaultYAxisOptions,
            [this.defaultTopAxisOptions, this.defaultRightAxisOptions,
                this.defaultBottomAxisOptions, this.defaultLeftAxisOptions][this.side],
            merge(
                defaultOptions[this.isXAxis ? 'xAxis' : 'yAxis'], // if set in setOptions (#1053)
                userOptions
            )
        );
    },
    /**
     * The default label formatter. The context is a special config object for the label.
     */
    defaultLabelFormatter: function () {
        let axis = this.axis,
            value = this.value,
            categories = axis.categories,
            dateTimeLabelFormat = this.dateTimeLabelFormat,
            numericSymbols = defaultOptions.lang.numericSymbols,
            i = numericSymbols && numericSymbols.length,
            multi,
            ret,
            // make sure the same symbol is added for all labels on a linear axis
            numericSymbolDetector = axis.isLog ? value : axis.tickInterval;
        if (categories) {
            ret = value;
        } else if (dateTimeLabelFormat) { // datetime axis
            ret = dateFormat(dateTimeLabelFormat, value);
        } else if (i && numericSymbolDetector >= 1000) {
            // Decide whether we should add a numeric symbol like k (thousands) or M (millions).
            // If we are to enable this in tooltip or other places as well, we can move this
            // logic to the numberFormatter and enable it by a parameter.
            while (i-- && ret === UNDEFINED) {
                multi = Math.pow(1000, i + 1);
                if (numericSymbolDetector >= multi && numericSymbols[i] !== null) {
                    ret = numberFormat(value / multi, -1) + numericSymbols[i];
                }
            }
        }
        if (ret === UNDEFINED) {
            if (value >= 1000) { // add thousands separators
                ret = numberFormat(value, 0);
            } else { // small numbers
                ret = numberFormat(value, -1);
            }
        }
        return ret;
    },
    /**
     * Get the minimum and maximum for the series of each axis
     */
    getSeriesExtremes: function () {
        let axis = this,
            chart = axis.chart,
            i;
        axis.hasVisibleSeries = false;
        // reset dataMin and dataMax in case we're redrawing
        axis.dataMin = axis.dataMax = null;
        // loop through this axis' series
        each(axis.seriesList, function (series) {
            if (series.visible || !chart.options.chart.ignoreHiddenSeries) {
                let seriesOptions = series.options,
                    negKey,
                    xData,
                    yData,
                    x,
                    y,
                    threshold = seriesOptions.threshold,
                    yDataLength,
                    activeYData = [],
                    activeCounter = 0;
                axis.hasVisibleSeries = true;
                // Validate threshold in logarithmic axes
                if (axis.isLog && threshold <= 0) {
                    threshold = seriesOptions.threshold = null;
                }
                // Get dataMin and dataMax for X axes
                if (axis.isXAxis) {
                    xData = series.xData;
                    if (xData.length) {
                        axis.dataMin = mathMin(pick(axis.dataMin, xData[0]), arrayMin(xData));
                        axis.dataMax = mathMax(pick(axis.dataMax, xData[0]), arrayMax(xData));
                    }
                // Get dataMin and dataMax for Y axes, as well as handle stacking and processed data
                } else {
                    let isNegative,
                        key,
                        cropped = series.cropped,
                        xExtremes = series.xAxis.getExtremes(),
                        //findPointRange,
                        //pointRange,
                        j,
                        hasModifyValue = !!series.modifyValue;
                    // processData can alter series.pointRange, so this goes after
                    //findPointRange = series.pointRange === null;
                    xData = series.processedXData;
                    yData = series.processedYData;
                    yDataLength = yData.length;
                    // loop over the non-null y values and read them into a local array
                    for (i = 0; i < yDataLength; i++) {
                        x = xData[i];
                        y = yData[i];
                        // Handle non null values
                        if (y !== null && y !== UNDEFINED) {
                            // general hook, used for Highstock compare values feature
                            if (hasModifyValue) {
                                y = series.modifyValue(y);
                            }
                            // for points within the visible range, including the first point outside the
                            // visible range, consider y extremes
                            if (cropped || ((xData[i + 1] || x) >= xExtremes.min && (xData[i - 1] || x) <= xExtremes.max)) {
                                j = y.length;
                                if (j) { // array, like ohlc or range data
                                    while (j--) {
                                        if (y[j] !== null) {
                                            activeYData[activeCounter++] = y[j];
                                        }
                                    }
                                } else {
                                    activeYData[activeCounter++] = y;
                                }
                            }
                        }
                    }
                    // Get the dataMin and dataMax so far. If percentage is used, the min and max are
                    // always 0 and 100. If the length of activeYData is 0, continue with null values.
                    if (activeYData.length) {
                        axis.dataMin = mathMin(pick(axis.dataMin, activeYData[0]), arrayMin(activeYData));
                        axis.dataMax = mathMax(pick(axis.dataMax, activeYData[0]), arrayMax(activeYData));
                    }
                    // Adjust to threshold
                    if (defined(threshold)) {
                        if (axis.dataMin >= threshold) {
                            axis.dataMin = threshold;
                            axis.ignoreMinPadding = true;
                        } else if (axis.dataMax < threshold) {
                            axis.dataMax = threshold;
                            axis.ignoreMaxPadding = true;
                        }
                    }
                }
            }
        });
    },
    /**
     * Translate from axis value to pixel position on the chart, or back
     *
     */
    translate: function (val, backwards, cvsCoord, old, handleLog, pointPlacementBetween) {
        let axis = this,
            axisLength = axis.len,
            sign = 1,
            cvsOffset = 0,
            localA = old ? axis.oldTransA : axis.transA,
            localMin = old ? axis.oldMin : axis.min,
            returnValue,
            postTranslate = axis.options.ordinal || (axis.isLog && handleLog);
        if (!localA) {
            localA = axis.transA;
        }
        if (cvsCoord) {
            sign *= -1; // canvas coordinates inverts the value
            cvsOffset = axisLength;
        }
        if (axis.reversed) { // reversed axis
            sign *= -1;
            cvsOffset -= sign * axisLength;
        }
        if (backwards) { // reverse translation
            if (axis.reversed) {
                val = axisLength - val;
            }
            returnValue = val / localA + localMin; // from chart pixel to value
            if (postTranslate) { // log and ordinal axes
                returnValue = axis.lin2val(returnValue);
            }
        } else { // normal translation, from axis value to pixel, relative to plot
            if (postTranslate) { // log and ordinal axes
                val = axis.val2lin(val);
            }
            returnValue = sign * (val - localMin) * localA + cvsOffset + (sign * axis.minPixelPadding) +
                (pointPlacementBetween ? localA * axis.pointRange / 2 : 0);
        }
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
        let axis = this,
            chart = axis.chart,
            axisLeft = axis.left,
            axisTop = axis.top,
            x1,
            y1,
            x2,
            y2,
            translatedValue = axis.translate(value, null, null, old),
            cHeight = (old && chart.oldChartHeight) || chart.chartHeight,
            cWidth = (old && chart.oldChartWidth) || chart.chartWidth,
            skip,
            transB = axis.transB;
        x1 = x2 = mathRound(translatedValue + transB);
        y1 = y2 = mathRound(cHeight - translatedValue - transB);
        if (isNaN(translatedValue)) { // no min or max
            skip = true;
        } else if (axis.horiz) {
            y1 = axisTop;
            y2 = cHeight - axis.bottom;
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
            chart.renderer.crispLine([M, x1, y1, L, x2, y2], lineWidth || 0);
    },
    /**
     * Create the path for a plot band
     */
    getPlotBandPath: function (from, to) {
        let toPath = this.getPlotLinePath(to),
            path = this.getPlotLinePath(from);
        if (path && toPath) {
            path.push(
                toPath[4],
                toPath[5],
                toPath[1],
                toPath[2]
            );
        } else { // outside the axis area
            path = null;
        }
        return path;
    },
    /**
     * Set the tick positions of a linear axis to round values like whole tens or every five.
     */
    getLinearTickPositions: function (tickInterval, min, max) {
        let pos,
            lastPos,
            roundedMin = correctFloat(mathFloor(min / tickInterval) * tickInterval),
            roundedMax = correctFloat(mathCeil(max / tickInterval) * tickInterval),
            tickPositions = [];
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
     * Set the tick positions of a logarithmic axis
     */
    getLogTickPositions: function (interval, min, max, minor) {
        let axis = this,
            options = axis.options,
            axisLength = axis.len;
        // Since we use this method for both major and minor ticks,
        // use a local variable and return the result
        let positions = [];
        // Reset
        if (!minor) {
            axis._minorAutoInterval = null;
        }
        // First case: All ticks fall on whole logarithms: 1, 10, 100 etc.
        if (interval >= 0.5) {
            interval = mathRound(interval);
            positions = axis.getLinearTickPositions(interval, min, max);
        // Second case: We need intermediary ticks. For example
        // 1, 2, 4, 6, 8, 10, 20, 40 etc.
        } else if (interval >= 0.08) {
            let roundedMin = mathFloor(min),
                intermediate,
                i,
                j,
                len,
                pos,
                lastPos,
                break2;
            if (interval > 0.3) {
                intermediate = [1, 2, 4];
            } else if (interval > 0.15) { // 0.2 equals five minor ticks per 1, 10, 100 etc
                intermediate = [1, 2, 4, 6, 8];
            } else { // 0.1 equals ten minor ticks per 1, 10, 100 etc
                intermediate = [1, 2, 3, 4, 5, 6, 7, 8, 9];
            }
            for (i = roundedMin; i < max + 1 && !break2; i++) {
                len = intermediate.length;
                for (j = 0; j < len && !break2; j++) {
                    pos = log2lin(lin2log(i) * intermediate[j]);
                    if (pos > min) {
                        positions.push(lastPos);
                    }
                    if (lastPos > max) {
                        break2 = true;
                    }
                    lastPos = pos;
                }
            }
        // Third case: We are so deep in between whole logarithmic values that
        // we might as well handle the tick positions like a linear axis. For
        // example 1.01, 1.02, 1.03, 1.04.
        } else {
            let realMin = lin2log(min),
                realMax = lin2log(max),
                tickIntervalOption = options[minor ? 'minorTickInterval' : 'tickInterval'],
                filteredTickIntervalOption = tickIntervalOption === 'auto' ? null : tickIntervalOption,
                tickPixelIntervalOption = options.tickPixelInterval / (minor ? 5 : 1),
                totalPixelLength = minor ? axisLength / axis.tickPositions.length : axisLength;
            interval = pick(
                filteredTickIntervalOption,
                axis._minorAutoInterval,
                (realMax - realMin) * tickPixelIntervalOption / (totalPixelLength || 1)
            );
            interval = normalizeTickInterval(
                interval,
                null,
                math.pow(10, mathFloor(math.log(interval) / math.LN10))
            );
            positions = map(axis.getLinearTickPositions(
                interval,
                realMin,
                realMax
            ), log2lin);
            if (!minor) {
                axis._minorAutoInterval = interval / 5;
            }
        }
        // Set the axis-level tickInterval variable
        if (!minor) {
            axis.tickInterval = interval;
        }
        return positions;
    },
    /**
     * Return the minor tick positions. For logarithmic axes, reuse the same logic
     * as for major ticks.
     */
    getMinorTickPositions: function () {
        let axis = this,
            options = axis.options,
            tickPositions = axis.tickPositions,
            minorTickInterval = axis.minorTickInterval;
        let minorTickPositions = [],
            pos,
            i,
            len;
        if (axis.isLog) {
            len = tickPositions.length;
            for (i = 1; i < len; i++) {
                minorTickPositions = minorTickPositions.concat(
                    axis.getLogTickPositions(minorTickInterval, tickPositions[i - 1], tickPositions[i], true)
                );
            }
        } else if (axis.isDatetimeAxis && options.minorTickInterval === 'auto') { // #1314
            minorTickPositions = minorTickPositions.concat(
                getTimeTicks(
                    normalizeTimeTickInterval(minorTickInterval),
                    axis.min,
                    axis.max,
                    options.startOfWeek
                )
            );
        } else {
            for (pos = axis.min + (tickPositions[0] - axis.min) % minorTickInterval; pos <= axis.max; pos += minorTickInterval) {
                minorTickPositions.push(pos);
            }
        }
        return minorTickPositions;
    },
    /**
     * Adjust the min and max for the minimum range. Keep in mind that the series data is
     * not yet processed, so we don't have information on data cropping and grouping, or
     * updated axis.pointRange or series.pointRange. The data can't be processed until
     * we have finally established min and max.
     */
    adjustForMinRange: function () {
        let axis = this,
            options = axis.options,
            min = axis.min,
            max = axis.max,
            zoomOffset,
            spaceAvailable = axis.dataMax - axis.dataMin >= axis.minRange,
            closestDataRange,
            i,
            distance,
            xData,
            loopLength,
            minArgs,
            maxArgs;
        // Set the automatic minimum range based on the closest point distance
        if (axis.isXAxis && axis.minRange === UNDEFINED && !axis.isLog) {
            if (defined(options.min) || defined(options.max)) {
                axis.minRange = null; // don't do this again
            } else {
                // Find the closest distance between raw data points, as opposed to
                // closestPointRange that applies to processed points (cropped and grouped)
                each(axis.seriesList, function (series) {
                    xData = series.xData;
                    loopLength = series.xIncrement ? 1 : xData.length - 1;
                    for (i = loopLength; i > 0; i--) {
                        distance = xData[i] - xData[i - 1];
                        if (closestDataRange === UNDEFINED || distance < closestDataRange) {
                            closestDataRange = distance;
                        }
                    }
                });
                axis.minRange = mathMin(closestDataRange * 5, axis.dataMax - axis.dataMin);
            }
        }
        // if minRange is exceeded, adjust
        if (max - min < axis.minRange) {
            let minRange = axis.minRange;
            zoomOffset = (minRange - max + min) / 2;
            // if min and max options have been set, don't go beyond it
            minArgs = [min - zoomOffset, pick(options.min, min - zoomOffset)];
            if (spaceAvailable) { // if space is available, stay within the data range
                minArgs[2] = axis.dataMin;
            }
            min = arrayMax(minArgs);
            maxArgs = [min + minRange, pick(options.max, min + minRange)];
            if (spaceAvailable) { // if space is availabe, stay within the data range
                maxArgs[2] = axis.dataMax;
            }
            max = arrayMin(maxArgs);
            // now if the max is adjusted, adjust the min back
            if (max - min < minRange) {
                minArgs[0] = max - minRange;
                minArgs[1] = pick(options.min, max - minRange);
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
        let axis = this,
            range = axis.max - axis.min,
            pointRange = 0,
            closestPointRange,
            minPointOffset = 0,
            pointRangePadding = 0,
            transA = axis.transA;
        // adjust translation for padding
        if (axis.isXAxis) {
            each(axis.seriesList, function (series) {
                let seriesPointRange = series.pointRange,
                    pointPlacement = series.options.pointPlacement,
                    seriesClosestPointRange = series.closestPointRange;
                pointRange = mathMax(pointRange, seriesPointRange);
                // minPointOffset is the value padding to the left of the axis in order to make
                // room for points with a pointRange, typically columns. When the pointPlacement option
                // is 'between' or 'on', this padding does not apply.
                minPointOffset = mathMax(
                    minPointOffset,
                    pointPlacement ? 0 : seriesPointRange / 2
                );
                // Determine the total padding needed to the length of the axis to make room for the
                // pointRange. If the series' pointPlacement is 'on', no padding is added.
                pointRangePadding = mathMax(
                    pointRangePadding,
                    pointPlacement === 'on' ? 0 : seriesPointRange
                );
                // Set the closestPointRange
                if (defined(seriesClosestPointRange)) {
                    closestPointRange = defined(closestPointRange) ?
                        mathMin(closestPointRange, seriesClosestPointRange) :
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
        axis.transB = axis.horiz ? axis.left : axis.bottom; // translation addend
        axis.minPixelPadding = transA * minPointOffset;
    },
    /**
     * Set the tick positions to round values and optionally extend the extremes
     * to the nearest tick
     */
    setTickPositions: function (secondPass) {
        let axis = this,
            chart = axis.chart,
            options = axis.options,
            isLog = axis.isLog,
            isDatetimeAxis = axis.isDatetimeAxis,
            isXAxis = axis.isXAxis,
            tickPositioner = axis.options.tickPositioner,
            magnitude,
            maxPadding = options.maxPadding,
            minPadding = options.minPadding,
            length,
            tickIntervalOption = options.tickInterval,
            minTickIntervalOption = options.minTickInterval,
            tickPixelIntervalOption = options.tickPixelInterval,
            tickPositions,
            categories = axis.categories;
        // initial min and max from the extreme data values
        axis.min = pick(axis.userMin, options.min, axis.dataMin);
        axis.max = pick(axis.userMax, options.max, axis.dataMax);
        if (isLog) {
            if (!secondPass && mathMin(axis.min, pick(axis.dataMin, axis.min)) <= 0) { // #978
                error(10, 1); // Can't plot negative values on log axis
            }
            axis.min = correctFloat(log2lin(axis.min)); // correctFloat cures #934
            axis.max = correctFloat(log2lin(axis.max));
        }
        // handle zoomed range
        if (axis.range) {
            axis.userMin = axis.min = mathMax(axis.min, axis.max - axis.range); // #618
            axis.userMax = axis.max;
            if (secondPass) {
                axis.range = null;  // don't use it when running setExtremes
            }
        }
        // adjust min and max for the minimum range
        axis.adjustForMinRange();
        // Pad the values to get clear of the chart's edges. To avoid tickInterval taking the padding
        // into account, we do this after computing tick interval (#1337).
        if (!categories && defined(axis.min) && defined(axis.max)) {
            length = axis.max - axis.min;
            if (length) {
                if (!defined(options.min) && !defined(axis.userMin) && minPadding && (axis.dataMin < 0 || !axis.ignoreMinPadding)) {
                    axis.min -= length * minPadding;
                }
                if (!defined(options.max) && !defined(axis.userMax)  && maxPadding && (axis.dataMax > 0 || !axis.ignoreMaxPadding)) {
                    axis.max += length * maxPadding;
                }
            }
        }
        // get tickInterval
        if (axis.min === axis.max || axis.min === undefined || axis.max === undefined) {
            axis.tickInterval = 1;
        } else {
            axis.tickInterval = pick(
                tickIntervalOption,
                (axis.max - axis.min) * tickPixelIntervalOption / (axis.len || 1)
            );
        }
        // Now we're finished detecting min and max, crop and group series data. This
        // is in turn needed in order to find tick positions in ordinal axes.
        if (isXAxis && !secondPass) {
            each(axis.seriesList, function (series) {
                series.processData(axis.min !== axis.oldMin || axis.max !== axis.oldMax);
            });
        }
        // set the translation factor used in translate function
        axis.setAxisTranslation(secondPass);
        // hook for ordinal axes and radial axes
        if (axis.beforeSetTickPositions) {
            axis.beforeSetTickPositions();
        }
        // hook for extensions, used in Highstock ordinal axes
        if (axis.postProcessTickInterval) {
            axis.tickInterval = axis.postProcessTickInterval(axis.tickInterval);
        }
        // Before normalizing the tick interval, handle minimum tick interval. This applies only if tickInterval is not defined.
        if (!tickIntervalOption && axis.tickInterval < minTickIntervalOption) {
            axis.tickInterval = minTickIntervalOption;
        }
        // for linear axes, get magnitude and normalize the interval
        if (!isDatetimeAxis && !isLog) { // linear
            magnitude = math.pow(10, mathFloor(math.log(axis.tickInterval) / math.LN10));
            if (!tickIntervalOption) {
                axis.tickInterval = normalizeTickInterval(axis.tickInterval, null, magnitude, options);
            }
        }
        // get minorTickInterval
        axis.minorTickInterval = options.minorTickInterval === 'auto' && axis.tickInterval ?
                axis.tickInterval / 5 : options.minorTickInterval;
        // find the tick positions
        axis.tickPositions = tickPositions = options.tickPositions || (tickPositioner && tickPositioner.apply(axis, [axis.min, axis.max]));
        if (!tickPositions) {
            if (isDatetimeAxis) {
                tickPositions = (axis.getNonLinearTimeTicks || getTimeTicks)(
                    normalizeTimeTickInterval(axis.tickInterval, options.units),
                    axis.min,
                    axis.max,
                    options.startOfWeek,
                    axis.ordinalPositions,
                    axis.closestPointRange,
                    true
                );
            } else if (isLog) {
                tickPositions = axis.getLogTickPositions(axis.tickInterval, axis.min, axis.max);
            } else {
                tickPositions = axis.getLinearTickPositions(axis.tickInterval, axis.min, axis.max);
            }
            axis.tickPositions = tickPositions;
        }
        // reset min/max or remove extremes based on start/end on tick
        let roundedMin = tickPositions[0],
            roundedMax = tickPositions[tickPositions.length - 1],
            minPointOffset = axis.minPointOffset || 0,
            singlePad;
        if (options.startOnTick) {
            axis.min = roundedMin;
        } else if (axis.min - minPointOffset > roundedMin) {
            tickPositions.shift();
        }
        if (options.endOnTick) {
            axis.max = roundedMax;
        } else if (axis.max + minPointOffset < roundedMax) {
            tickPositions.pop();
        }
        // When there is only one point, or all points have the same value on this axis, then min
        // and max are equal and tickPositions.length is 1. In this case, add some padding
        // in order to center the point, but leave it with one tick. #1337.
        if (tickPositions.length === 1) {
            singlePad = 1e-9; // The lowest possible number to avoid extra padding on columns
            axis.min -= singlePad;
            axis.max += singlePad;
        }
    },
    /**
     * Set the max ticks of either the x and y axis collection
     */
    setMaxTicks: function () {
        let chart = this.chart,
            maxTicks = chart.maxTicks,
            tickPositions = this.tickPositions,
            xOrY = this.xOrY;
        if (!maxTicks) { // first call, or maxTicks have been reset after a zoom operation
            maxTicks = {
                x: 0,
                y: 0
            };
        }
        if (!this.isDatetimeAxis && tickPositions.length > maxTicks[xOrY] && this.options.alignTicks !== false) {
            maxTicks[xOrY] = tickPositions.length;
        }
        chart.maxTicks = maxTicks;
    },
    /**
     * When using multiple axes, adjust the number of ticks to match the highest
     * number of ticks in that group
     */
    adjustTickAmount: function () {
        let axis = this,
            chart = axis.chart,
            xOrY = axis.xOrY,
            tickPositions = axis.tickPositions,
            maxTicks = chart.maxTicks;
        if (maxTicks && maxTicks[xOrY] && !axis.isDatetimeAxis && !axis.categories && axis.options.alignTicks !== false) { // only apply to linear scale
            let oldTickAmount = axis.tickAmount,
                calculatedTickAmount = tickPositions.length,
                tickAmount;
            // set the axis-level tickAmount to use below
            axis.tickAmount = tickAmount = maxTicks[xOrY];
            if (calculatedTickAmount < tickAmount) {
                while (tickPositions.length < tickAmount) {
                    tickPositions.push(correctFloat(
                        tickPositions[tickPositions.length - 1] + axis.tickInterval
                    ));
                }
                axis.transA *= (calculatedTickAmount - 1) / (tickAmount - 1);
                axis.max = tickPositions[tickPositions.length - 1];
            }
            if (defined(oldTickAmount) && tickAmount !== oldTickAmount) {
                axis.isDirty = true;
            }
        }
    },
    /**
     * Set the scale based on data min and max, user set min and max or options
     *
     */
    setScale: function () {
        let axis = this,
            type,
            i,
            isDirtyData,
            isDirtyAxisLength;
        axis.oldMin = axis.min;
        axis.oldMax = axis.max;
        axis.oldAxisLength = axis.len;
        // set the new axisLength
        axis.setAxisSize();
        //axisLength = horiz ? axisWidth : axisHeight;
        isDirtyAxisLength = axis.len !== axis.oldAxisLength;
        // is there new data?
        each(axis.seriesList, function (series) {
            if (series.isDirtyData || series.isDirty ||
                    series.xAxis.isDirty) { // when x axis is dirty, we need new data extremes for y as well
                isDirtyData = true;
            }
        });
        // do we really need to go through all this?
        if (isDirtyAxisLength || isDirtyData ||
            axis.userMin !== axis.oldUserMin || axis.userMax !== axis.oldUserMax) {
            // get data extremes if needed
            axis.getSeriesExtremes();
            // get fixed positions based on tickInterval
            axis.setTickPositions();
            // record old values to decide whether a rescale is necessary later on (#540)
            axis.oldUserMin = axis.userMin;
            axis.oldUserMax = axis.userMax;
            // Mark as dirty if it is not already set to dirty and extremes have changed. #595.
            if (!axis.isDirty) {
                axis.isDirty = isDirtyAxisLength || axis.min !== axis.oldMin || axis.max !== axis.oldMax;
            }
        }
        // Set the maximum tick amount
        axis.setMaxTicks();
    },
    /**
     * Set the extremes and optionally redraw
     * @param {Number} newMin
     * @param {Number} newMax
     * @param {Boolean} redraw
     * @param {Boolean|Object} animation Whether to apply animation, and optionally animation
     *    configuration
     * @param {Object} eventArguments
     *
     */
    setExtremes: function (newMin, newMax, redraw, animation, eventArguments) {
        let axis = this,
            chart = axis.chart;
        redraw = pick(redraw, true); // defaults to true
        // Extend the arguments with min and max
        eventArguments = extend(eventArguments, {
            min: newMin,
            max: newMax
        });
        // Fire the event
        fireEvent(axis, 'setExtremes', eventArguments, function () { // the default event handler
            axis.userMin = newMin;
            axis.userMax = newMax;
            // Mark for running afterSetExtremes
            axis.isDirtyExtremes = true;
            // redraw
            if (redraw) {
                chart.redraw(animation);
            }
        });
    },
    /**
     * Overridable method for zooming chart. Pulled out in a separate method to allow overriding
     * in stock charts.
     */
    zoom: function (newMin, newMax) {
        this.setExtremes(newMin, newMax, false, UNDEFINED, { trigger: 'zoom' });
        return true;
    },
    /**
     * Update the axis metrics
     */
    setAxisSize: function () {
        let axis = this,
            chart = axis.chart,
            options = axis.options;
        let offsetLeft = options.offsetLeft || 0,
            offsetRight = options.offsetRight || 0;
        // basic values
        // expose to use in Series object and navigator
        axis.left = pick(options.left, chart.plotLeft + offsetLeft);
        axis.top = pick(options.top, UI_CHART_PLOT_TOP);
        axis.width = pick(options.width, chart.plotWidth - offsetLeft + offsetRight);
        axis.height = pick(options.height, chart.plotHeight);
        axis.bottom = chart.chartHeight - axis.height - axis.top;
        axis.right = chart.chartWidth - axis.width - axis.left;
        axis.len = mathMax(axis.horiz ? axis.width : axis.height, 0); // mathMax fixes #905
    },
    /**
     * Get the actual axis extremes
     */
    getExtremes: function () {
        let axis = this,
            isLog = axis.isLog;
        return {
            min: isLog ? correctFloat(lin2log(axis.min)) : axis.min,
            max: isLog ? correctFloat(lin2log(axis.max)) : axis.max,
            dataMin: axis.dataMin,
            dataMax: axis.dataMax,
            userMin: axis.userMin,
            userMax: axis.userMax
        };
    },
    /**
     * Get the zero plane either based on zero or on the min or max value.
     * Used in bar and area plots
     */
    getThreshold: function (threshold) {
        let axis = this,
            isLog = axis.isLog;
        let realMin = isLog ? lin2log(axis.min) : axis.min,
            realMax = isLog ? lin2log(axis.max) : axis.max;
        if (realMin > threshold || threshold === null) {
            threshold = realMin;
        } else if (realMax < threshold) {
            threshold = realMax;
        }
        return axis.translate(threshold, 0, 1, 0, 1);
    },
    /**
     * Add a plot band or plot line after render time
     *
     * @param options {Object} The plotBand or plotLine configuration object
     */
    addPlotBandOrLine: function (options) {
        let obj = new PlotLineOrBand(this, options).render();
        this.plotLinesAndBands.push(obj);
        return obj;
    },
    /**
     * Render the tick labels to a preliminary position to get their sizes
     */
    getOffset: function () {
        let axis = this,
            chart = axis.chart,
            renderer = chart.renderer,
            options = axis.options,
            tickPositions = axis.tickPositions,
            ticks = axis.ticks,
            horiz = axis.horiz,
            side = axis.side,
            hasData,
            showAxis,
            labelOptions = options.labels,
            labelOffset = 0, // reset
            axisOffset = chart.axisOffset,
            directionFactor = [-1, 1, 1, -1][side],
            n;
        // For reuse in Axis.render
        axis.hasData = hasData = (axis.hasVisibleSeries || (defined(axis.min) && defined(axis.max) && !!tickPositions));
        axis.showAxis = showAxis = hasData || pick(options.showEmpty, true);
        // Create the axisGroup and gridGroup elements on first iteration
        if (!axis.axisGroup) {
            axis.gridGroup = renderer.g('grid')
                .attr({ zIndex: options.gridZIndex || 1 })
                .add();
            axis.axisGroup = renderer.g('axis')
                .attr({ zIndex: options.zIndex || 2 })
                .add();
            axis.labelGroup = renderer.g('axis-labels')
                .attr({ zIndex: labelOptions.zIndex || 7 })
                .add();
        }
        if (hasData) {
            each(tickPositions, function (pos) {
                if (!ticks[pos]) {
                    ticks[pos] = new Tick(axis, pos);
                } else {
                    ticks[pos].addLabel(); // update labels depending on tick interval
                }
            });
            each(tickPositions, function (pos) {
                // left side must be align: right and right side must have align: left for labels
                if (side === 0 || side === 2 || { 1: 'left', 3: 'right' }[side] === labelOptions.align) {
                    // get the highest offset
                    labelOffset = mathMax(
                        ticks[pos].getLabelSize(),
                        labelOffset
                    );
                }
            });
            if (axis.staggerLines) {
                labelOffset += (axis.staggerLines - 1) * 16;
            }
        } else { // doesn't have data
            for (n in ticks) {
                ticks[n].destroy();
                delete ticks[n];
            }
        }
        // handle automatic or user set offset
        axis.offset = directionFactor * pick(options.offset, axisOffset[side]);
        // !!important!!
        axisOffset[side] = mathMax(
            axisOffset[side],
            pick(
                labelOffset +
                (side !== 2 && labelOffset && directionFactor * options.labels[horiz ? 'y' : 'x'])
            ) + directionFactor * axis.offset
        );
    },
    /**
     * Get the path for the axis line
     */
    getLinePath: function (lineWidth) {
        let chart = this.chart,
            opposite = this.opposite,
            offset = this.offset,
            horiz = this.horiz,
            lineLeft = this.left + (opposite ? this.width : 0) + offset,
            lineTop = chart.chartHeight - this.bottom - (opposite ? this.height : 0) + offset;
        this.lineTop = lineTop; // used by flag series
        return chart.renderer.crispLine([
                M,
                horiz ?
                    this.left :
                    lineLeft,
                horiz ?
                    lineTop :
                    this.top,
                L,
                horiz ?
                    chart.chartWidth - this.right :
                    lineLeft,
                horiz ?
                    lineTop :
                    chart.chartHeight - this.bottom
            ], lineWidth);
    },
    /**
     * Render the axis
     */
    render: function () {
        let axis = this,
            chart = axis.chart,
            renderer = chart.renderer,
            options = axis.options,
            isLog = axis.isLog,
            tickPositions = axis.tickPositions,
            ticks = axis.ticks,
            minorTicks = axis.minorTicks,
            alternateBands = axis.alternateBands,
            tickmarkOffset = axis.tickmarkOffset,
            lineWidth = options.lineWidth,
            linePath,
            hasRendered = chart.hasRendered,
            slideInTicks = hasRendered && defined(axis.oldMin) && !isNaN(axis.oldMin),
            hasData = axis.hasData,
            showAxis = axis.showAxis,
            from,
            to;
        // If the series has data draw the ticks. Else only the line and title
        if (hasData) {
            // minor ticks
            if (axis.minorTickInterval && !axis.categories) {
                each(axis.getMinorTickPositions(), function (pos) {
                    if (!minorTicks[pos]) {
                        minorTicks[pos] = new Tick(axis, pos, 'minor');
                    }
                    // render new ticks in old position
                    if (slideInTicks && minorTicks[pos].isNew) {
                        minorTicks[pos].render(null, true);
                    }
                    minorTicks[pos].isActive = true;
                    minorTicks[pos].render();
                });
            }
            // Major ticks. Pull out the first item and render it last so that
            // we can get the position of the neighbour label. #808.
            if (tickPositions.length) { // #1300
                each(tickPositions.slice(1).concat([tickPositions[0]]), function (pos, i) {
                    // Reorganize the indices
                    i = (i === tickPositions.length - 1) ? 0 : i + 1;
                    // linked axes need an extra check to find out if
                    if ((pos >= axis.min && pos <= axis.max)) {
                        if (!ticks[pos]) {
                            ticks[pos] = new Tick(axis, pos);
                        }
                        // render new ticks in old position
                        if (slideInTicks && ticks[pos].isNew) {
                            ticks[pos].render(i, true);
                        }
                        ticks[pos].isActive = true;
                        ticks[pos].render(i);
                    }
                });
            }
            // alternate grid color
            if (axis.isXAxis) {
                each(tickPositions, function (pos, i) {
                    if (i % 2 === 0 && pos < axis.max) {
                        if (!alternateBands[pos]) {
                            alternateBands[pos] = new PlotLineOrBand(axis);
                        }
                        from = pos + tickmarkOffset; // #949
                        to = tickPositions[i + 1] !== UNDEFINED ? tickPositions[i + 1] + tickmarkOffset : axis.max;
                        alternateBands[pos].options = {
                            from: isLog ? lin2log(from) : from,
                            to: isLog ? lin2log(to) : to,
                            color: "#f0f0ff"
                        };
                        alternateBands[pos].render();
                        alternateBands[pos].isActive = true;
                    }
                });
            }
            // custom plot lines and bands
            if (!axis._addedPlotLB) { // only first time
                each((options.plotLines || []).concat(options.plotBands || []), function (plotLineOptions) {
                    //plotLinesAndBands.push(new PlotLineOrBand(plotLineOptions).render());
                    axis.addPlotBandOrLine(plotLineOptions);
                });
                axis._addedPlotLB = true;
            }
        } // end if hasData
        // remove inactive ticks
        each([ticks, minorTicks, alternateBands], function (coll) {
            let pos;
            for (pos in coll) {
                if (!coll[pos].isActive) {
                    coll[pos].destroy();
                    delete coll[pos];
                } else {
                    coll[pos].isActive = false; // reset
                }
            }
        });
        // Static items. As the axis group is cleared on subsequent calls
        // to render, these items are added outside the group.
        // axis line
        if (lineWidth) {
            linePath = axis.getLinePath(lineWidth);
            if (!axis.axisLine) {
                axis.axisLine = renderer.path(linePath)
                    .attr({
                        stroke: options.lineColor,
                        'stroke-width': lineWidth,
                        zIndex: 7
                    })
                    .add(axis.axisGroup);
            } else {
                axis.axisLine.animate({ d: linePath });
            }
            // show or hide the line depending on options.showEmpty
            axis.axisLine[showAxis ? 'show' : 'hide']();
        }
        axis.isDirty = false;
    },
    /**
     * Remove a plot band or plot line from the chart by id
     * @param {Object} id
     */
    removePlotBandOrLine: function (id) {
        let plotLinesAndBands = this.plotLinesAndBands,
            i = plotLinesAndBands.length;
        while (i--) {
            if (plotLinesAndBands[i].id === id) {
                plotLinesAndBands[i].destroy();
            }
        }
    },
    /**
     * Redraw the axis to reflect changes in the data or axis extremes
     */
    redraw: function () {
        let axis = this,
            chart = axis.chart;
        // hide tooltip and hover states
        if (chart.tracker.resetTracker) {
            chart.tracker.resetTracker(true);
        }
        // render the axis
        axis.render();
        // move plot lines and bands
        each(axis.plotLinesAndBands, function (plotLine) {
            plotLine.render();
        });
        // mark associated series as dirty and ready for redraw
        each(axis.seriesList, function (series) {
            series.isDirty = true;
        });
    },
    /**
     * Set new axis categories and optionally redraw
     * @param {Array} newCategories
     * @param {Boolean} doRedraw
     */
    setCategories: function (newCategories, doRedraw) {
        let axis = this,
            chart = axis.chart;
        // set the categories
        axis.categories = axis.userOptions.categories = newCategories;
        // force reindexing tooltips
        each(axis.seriesList, function (series) {
            series.translate();
            series.setTooltipPoints(true);
        });
        // optionally redraw
        axis.isDirty = true;
        if (pick(doRedraw, true)) {
            chart.redraw();
        }
    },
    /**
     * Destroys an Axis instance.
     */
    destroy: function () {
        let axis = this;
        // Remove the events
        removeEvent(axis);
        // Destroy collections
        each([axis.ticks, axis.minorTicks, axis.alternateBands, axis.plotLinesAndBands], function (coll) {
            destroyObjectProperties(coll);
        });
        // Destroy local variables
        each(['axisLine', 'axisGroup', 'gridGroup', 'labelGroup'], function (prop) {
            if (axis[prop]) {
                axis[prop] = axis[prop].destroy();
            }
        });
    }
}; // end Axis
/**
 * The tooltip object
 * @param {Object} chart The chart instance
 * @param {Object} options Tooltip options
 */
function Tooltip(chart, options) {
    let borderWidth = options.borderWidth,
        style = options.style,
        padding = pInt(style.padding);
    // Save the chart and options
    this.chart = chart;
    this.options = options;
    // Keep track of the current series
    //this.currentSeries = UNDEFINED;
    // List of crosshairs
    this.crosshairs = [];
    // Current values of x and y when animating
    this.now = { x: 0, y: 0 };
    // The tooltip is initially hidden
    this.isHidden = true;
    // create the label
    this.label = chart.renderer.label('', 0, 0, options.shape, null, null, options.useHTML, null, 'tooltip')
        .attr({
            padding: padding,
            fill: options.backgroundColor,
            'stroke-width': borderWidth,
            r: options.borderRadius,
            zIndex: 8
        })
        .css(style)
        .css({ padding: 0 }) // Remove it from VML, the padding is applied as an attribute instead (#1117)
        .hide()
        .add();
    // When using canVG the shadow shows up as a gray circle
    // even if the tooltip is hidden.
    //!! if (!useCanVG) {
    this.label.shadow(options.shadow);
    //!! }
    // Public property for getting the shared state.
    this.shared = options.shared;
}
Tooltip.prototype = {
    /**
     * Destroy the tooltip and its elements.
     */
    destroy: function () {
        each(this.crosshairs, function (crosshair) {
            if (crosshair) {
                crosshair.destroy();
            }
        });
        // Destroy and clear local variables
        if (this.label) {
            this.label = this.label.destroy();
        }
    },
    /**
     * Provide a soft movement for the tooltip
     *
     * @param {Number} x
     * @param {Number} y
     * @private
     */
    move: function (x, y, anchorX, anchorY) {
        let tooltip = this,
            now = tooltip.now,
            animate = tooltip.options.animation !== false && !tooltip.isHidden;
        // get intermediate values for animation
        extend(now, {
            x: animate ? (2 * now.x + x) / 3 : x,
            y: animate ? (now.y + y) / 2 : y,
            anchorX: animate ? (2 * now.anchorX + anchorX) / 3 : anchorX,
            anchorY: animate ? (now.anchorY + anchorY) / 2 : anchorY
        });
        // move to the intermediate value
        tooltip.label.attr(now);
        // run on next tick of the mouse tracker
        if (animate && (mathAbs(x - now.x) > 1 || mathAbs(y - now.y) > 1)) {
            // never allow two timeouts
            clearTimeout(this.tooltipTimeout);
            // set the fixed interval ticking for the smooth tooltip
            this.tooltipTimeout = setTimeout(function () {
                // The interval function may still be running during destroy, so check that the chart is really there before calling.
                if (tooltip) {
                    tooltip.move(x, y, anchorX, anchorY);
                }
            }, 32);
        }
    },
    /**
     * Hide the tooltip
     */
    hide: function () {
        if (!this.isHidden) {
            let hoverPoints = this.chart.hoverPoints;
            this.label.hide();
            // hide previous hoverPoints and set new
            if (hoverPoints) {
                each(hoverPoints, function (point) {
                    point.setState();
                });
            }
            this.chart.hoverPoints = null;
            this.isHidden = true;
        }
    },
    /**
     * Hide the crosshairs
     */
    hideCrosshairs: function () {
        each(this.crosshairs, function (crosshair) {
            if (crosshair) {
                crosshair.hide();
            }
        });
    },
    /**
     * Extendable method to get the anchor position of the tooltip
     * from a point or set of points
     */
    getAnchor: function (points, mouseEvent) {
        let ret,
            chart = this.chart,
            plotX = 0,
            plotY = 0,
            yAxis;
        points = splat(points);
        // Pie uses a special tooltipPos
        ret = points[0].tooltipPos;
        // When shared, use the average position
        if (!ret) {
            each(points, function (point) {
                yAxis = point.seriesList.yAxis;
                plotX += point.plotX;
                plotY += (point.plotLow ? (point.plotLow + point.plotHigh) / 2 : point.plotY) +
                    (yAxis ? yAxis.top - UI_CHART_PLOT_TOP : 0); // #1151
            });
            plotX /= points.length;
            plotY /= points.length;
            ret = [
                plotX, plotY
            ];
        }
        return map(ret, mathRound);
    },
    /**
     * Place the tooltip in a chart without spilling over
     * and not covering the point it self.
     */
    getPosition: function (boxWidth, boxHeight, point) {
        // Set up the variables
        let chart = this.chart,
            plotLeft = chart.plotLeft,
            plotWidth = chart.plotWidth,
            plotHeight = chart.plotHeight,
            distance = pick(this.options.distance, 12),
            pointX = point.plotX,
            pointY = point.plotY,
            x = pointX + plotLeft + (-boxWidth - distance),
            y = pointY - boxHeight + UI_CHART_PLOT_TOP + 15, // 15 means the point is 15 pixels up from the bottom of the tooltip
            alignedRight;
        // It is too far to the left, adjust it
        if (x < 7) {
            x = plotLeft + mathMax(pointX, 0) + distance;
        }
        // Test to see if the tooltip is too far to the right,
        // if it is, move it back to be inside and then up to not cover the point.
        if ((x + boxWidth) > (plotLeft + plotWidth)) {
            x -= (x + boxWidth) - (plotLeft + plotWidth);
            y = pointY - boxHeight + UI_CHART_PLOT_TOP - distance;
            alignedRight = true;
        }
        // If it is now above the plot area, align it to the top of the plot area
        if (y < UI_CHART_PLOT_TOP + 5) {
            y = UI_CHART_PLOT_TOP + 5;
            // If the tooltip is still covering the point, move it below instead
            if (alignedRight && pointY >= y && pointY <= (y + boxHeight)) {
                y = pointY + UI_CHART_PLOT_TOP + distance; // below
            }
        }
        // Now if the tooltip is below the chart, move it up. It's better to cover the
        // point than to disappear outside the chart. #834.
        if (y + boxHeight > UI_CHART_PLOT_TOP + plotHeight) {
            y = mathMax(UI_CHART_PLOT_TOP, UI_CHART_PLOT_TOP + plotHeight - boxHeight - distance); // below
        }
        return {x: x, y: y};
    },
    /**
     * Refresh the tooltip's text and position.
     * @param {Object} point
     */
    refresh: function (point, mouseEvent) {
        let tooltip = this,
            chart = tooltip.chart,
            label = tooltip.label,
            options = tooltip.options;
        /**
         * In case no user defined formatter is given, this will be used
         */
        function defaultFormatter() {
            let pThis = this,
                items = pThis.points || splat(pThis),
                series = items[0].seriesList,
                s;
            // build the header
            s = [series.tooltipHeaderFormatter(items[0].key)];
            // build the values
            each(items, function (item) {
                series = item.seriesList;
                s.push((series.tooltipFormatter && series.tooltipFormatter(item)) ||
                    item.point.tooltipFormatter(series.tooltipOptions.pointFormat));
            });
            // footer
            s.push(options.footerFormat || '');
            return s.join('');
        }
        let x,
            y,
            show,
            anchor,
            textConfig = {},
            text,
            pointConfig = [],
            formatter = options.formatter || defaultFormatter,
            hoverPoints = chart.hoverPoints,
            placedTooltipPoint,
            borderColor,
            crosshairsOptions = options.crosshairs,
            currentSeries;
        // get the reference point coordinates (pie charts use tooltipPos)
        anchor = tooltip.getAnchor(point, mouseEvent);
        x = anchor[0];
        y = anchor[1];
        // single point tooltip
        textConfig = point.getLabelConfig();
        text = formatter.call(textConfig);
        // register the current series
        currentSeries = point.seriesList;
        // For line type series, hide tooltip if the point falls outside the plot
        show = !currentSeries.isCartesian || currentSeries.tooltipOutsidePlot || chart.isInsidePlot(x, y);
        // update the inner HTML
        if (text === false || !show) {
            this.hide();
        } else {
            // show it
            if (tooltip.isHidden) {
                label.show();
            }
            // update text
            label.attr({
                text: text
            });
            // set the stroke color of the box
            borderColor = options.borderColor || point.color || currentSeries.color || '#606060';
            label.attr({
                stroke: borderColor
            });
            placedTooltipPoint = (options.positioner || tooltip.getPosition).call(
                tooltip,
                label.width,
                label.height,
                { plotX: x, plotY: y }
            );
            // do the move
            tooltip.move(
                mathRound(placedTooltipPoint.x),
                mathRound(placedTooltipPoint.y),
                x + chart.plotLeft,
                y + UI_CHART_PLOT_TOP
            );
            tooltip.isHidden = false;
        }
        // crosshairs
        if (crosshairsOptions) {
            crosshairsOptions = splat(crosshairsOptions); // [x, y]
            let path,
                i = crosshairsOptions.length,
                attribs,
                axis;
            while (i--) {
                axis = point.seriesList[i ? 'yAxis' : 'xAxis'];
                if (crosshairsOptions[i] && axis) {
                    path = axis.getPlotLinePath(
                        i ? point.y : point.x, // #814
                        1
                    );
                    if (tooltip.crosshairs[i]) {
                        tooltip.crosshairs[i].attr({ d: path, visibility: VISIBLE });
                    } else {
                        attribs = {
                            'stroke-width': crosshairsOptions[i].width || 1,
                            stroke: crosshairsOptions[i].color || '#C0C0C0',
                            zIndex: crosshairsOptions[i].zIndex || 2
                        };
                        if (crosshairsOptions[i].dashStyle) {
                            attribs.dashstyle = crosshairsOptions[i].dashStyle;
                        }
                        tooltip.crosshairs[i] = chart.renderer.path(path)
                            .attr(attribs)
                            .add();
                    }
                }
            }
        }
        fireEvent(chart, 'tooltipRefresh', {
                text: text,
                x: x + chart.plotLeft,
                y: y + UI_CHART_PLOT_TOP,
                borderColor: borderColor
            });
    }
};
/**
 * The mouse tracker object
 * @param {Object} chart The Chart instance
 * @param {Object} options The root options object
 */
function MouseTracker(chart, options) {
    let zoomType = options.chart.zoomType;
    // Zoom status
    this.zoomX = /x/.test(zoomType);
    this.zoomY = /y/.test(zoomType);
    // Store reference to options
    this.options = options;
    // Reference to the chart
    this.chart = chart;
    // The interval id
    //this.tooltipTimeout = UNDEFINED;
    // The cached x hover position
    //this.hoverX = UNDEFINED;
    // The chart position
    //this.chartPosition = UNDEFINED;
    // The selection marker element
    //this.selectionMarker = UNDEFINED;
    // False or a value > 0 if a dragging operation
    //this.mouseDownX = UNDEFINED;
    //this.mouseDownY = UNDEFINED;
    this.init(chart, options.tooltip);
}
MouseTracker.prototype = {
    /**
     * Add crossbrowser support for chartX and chartY
     * @param {Object} e The event object in standard browsers
     */
    normalizeMouseEvent: function (e) {
        let chartPosition,
            chartX,
            chartY,
            ePos;
        if (!e.target) {
            e.target = e.srcElement;
        }
        // Framework specific normalizing (#1165)
        e = washMouseEvent(e);
        // iOS
        ePos = e.touches ? e.touches.item(0) : e;
        // get mouse position
        this.chartPosition = chartPosition = offset(this.chart.container);
        chartX = ePos.pageX - chartPosition.left;
        chartY = ePos.pageY - chartPosition.top;
        return extend(e, {
            chartX: mathRound(chartX),
            chartY: mathRound(chartY)
        });
    },
    /**
     * Get the click position in terms of axis values.
     *
     * @param {Object} e A mouse event
     */
    getMouseCoordinates: function (e) {
        let coordinates = {
                xAxis: [],
                yAxis: []
            },
            chart = this.chart;
        each(chart.axisList, function (axis) {
            let isXAxis = axis.isXAxis,
                isHorizontal = isXAxis;
            coordinates[isXAxis ? 'xAxis' : 'yAxis'].push({
                axis: axis,
                value: axis.translate(
                    (isHorizontal ?
                        e.chartX - chart.plotLeft :
                        axis.top + axis.len - e.chartY) - axis.minPixelPadding, // #1051
                    true
                )
            });
        });
        return coordinates;
    },
    /**
     * Return the index in the tooltipPoints array, corresponding to pixel position in
     * the plot area.
     */
    getIndex: function (e) {
        let chart = this.chart;
        return e.chartX - chart.plotLeft;
    },
    /**
     * With line type charts with a single tracker, get the point closest to the mouse
     */
    onmousemove: function (e) {
        let mouseTracker = this,
            chart = mouseTracker.chart,
            series = chart.seriesList,
            tooltip = chart.tooltip,
            point,
            points,
            hoverPoint = chart.hoverPoint,
            hoverSeries = chart.hoverSeries,
            i,
            j,
            distance = chart.chartWidth,
            index = mouseTracker.getIndex(e);
        // separate tooltip and general mouse events
        if (hoverSeries && hoverSeries.tracker) { // only use for line-type series with common tracker
            // get the point
            point = hoverSeries.tooltipPoints[index];
            // a new point is hovered, refresh the tooltip
            if (point && point !== hoverPoint) {
                // trigger the events
                point.onMouseOver();
            }
        }
    },
    /**
     * Reset the tracking by hiding the tooltip, the hover series state and the hover point
     *
     * @param allowMove {Boolean} Instead of destroying the tooltip altogether, allow moving it if possible
     */
    resetTracker: function (allowMove) {
        let mouseTracker = this,
            chart = mouseTracker.chart,
            hoverSeries = chart.hoverSeries,
            hoverPoint = chart.hoverPoint,
            tooltip = chart.tooltip,
            tooltipPoints = hoverPoint;
        // Narrow in allowMove
        allowMove = allowMove && tooltip && tooltipPoints;
        // Check if the points have moved outside the plot area, #1003
        if (allowMove && splat(tooltipPoints)[0].plotX === UNDEFINED) {
            allowMove = false;
        }
        // Just move the tooltip, #349
        if (allowMove) {
            tooltip.refresh(tooltipPoints);
        // Full reset
        } else {
            if (hoverPoint) {
                hoverPoint.onMouseOut();
            }
            if (hoverSeries) {
                hoverSeries.onMouseOut();
            }
            if (tooltip) {
                tooltip.hide();
                tooltip.hideCrosshairs();
            }
            mouseTracker.hoverX = null;
        }
    },
    /**
     * Set the JS events on the container element
     */
    setDOMEvents: function () {
        let lastWasOutsidePlot = true,
            mouseTracker = this,
            chart = mouseTracker.chart,
            container = chart.container,
            hasDragged,
            zoomHor = (mouseTracker.zoomX),
            zoomVert = (mouseTracker.zoomY);
        /**
         * Mouse up or outside the plot area
         */
        function drop() {
            if (mouseTracker.selectionMarker) {
                let selectionData = {
                        xAxis: [],
                        yAxis: []
                    },
                    selectionBox = mouseTracker.selectionMarker.getBBox(),
                    selectionLeft = selectionBox.x - chart.plotLeft,
                    selectionTop = selectionBox.y - UI_CHART_PLOT_TOP,
                    runZoom;
                // a selection has been made
                if (hasDragged) {
                    // record each axis' min and max
                    each(chart.axisList, function (axis) {
                        if (axis.options.zoomEnabled !== false) {
                            let isXAxis = axis.isXAxis,
                                isHorizontal = isXAxis,
                                selectionMin = axis.translate(
                                    isHorizontal ?
                                        selectionLeft :
                                        chart.plotHeight - selectionTop - selectionBox.height,
                                    true,
                                    0,
                                    0,
                                    1
                                ),
                                selectionMax = axis.translate(
                                    (isHorizontal ?
                                            selectionLeft + selectionBox.width :
                                            chart.plotHeight - selectionTop) -
                                        2 * axis.minPixelPadding, // #875
                                    true,
                                    0,
                                    0,
                                    1
                                );
                                if (!isNaN(selectionMin) && !isNaN(selectionMax)) { // #859
                                    selectionData[isXAxis ? 'xAxis' : 'yAxis'].push({
                                        axis: axis,
                                        min: mathMin(selectionMin, selectionMax), // for reversed axes,
                                        max: mathMax(selectionMin, selectionMax)
                                    });
                                    runZoom = true;
                                }
                        }
                    });
                    if (runZoom) {
                        fireEvent(chart, 'selection', selectionData, function (args) { chart.zoom(args); });
                    }
                }
                mouseTracker.selectionMarker = mouseTracker.selectionMarker.destroy();
            }
            if (chart) { // it may be destroyed on mouse up - #877
                css(container, { cursor: 'auto' });
                chart.cancelClick = hasDragged; // #370
                chart.mouseIsDown = hasDragged = false;
            }
            removeEvent(doc, 'mouseup', drop);
            if (hasTouch) {
                removeEvent(doc, 'touchend', drop);
            }
        }
        /**
         * Special handler for mouse move that will hide the tooltip when the mouse leaves the plotarea.
         */
        mouseTracker.hideTooltipOnMouseMove = function (e) {
            // Get e.pageX and e.pageY back in MooTools
            e = washMouseEvent(e);
            // If we're outside, hide the tooltip
            if (mouseTracker.chartPosition && chart.hoverSeries && chart.hoverSeries.isCartesian &&
                !chart.isInsidePlot(e.pageX - mouseTracker.chartPosition.left - chart.plotLeft,
                e.pageY - mouseTracker.chartPosition.top - UI_CHART_PLOT_TOP)) {
                    mouseTracker.resetTracker();
            }
        };
        /**
         * When mouse leaves the container, hide the tooltip.
         */
        mouseTracker.hideTooltipOnMouseLeave = function () {
            mouseTracker.resetTracker();
            mouseTracker.chartPosition = null; // also reset the chart position, used in #149 fix
        };
        /*
         * Record the starting position of a dragoperation
         */
        container.onmousedown = function (e) {
            e = mouseTracker.normalizeMouseEvent(e);
            // issue #295, dragging not always working in Firefox
            if (e.type.indexOf('touch') === -1 && e.preventDefault) {
                e.preventDefault();
            }
            // record the start position
            chart.mouseIsDown = true;
            chart.cancelClick = false;
            chart.mouseDownX = mouseTracker.mouseDownX = e.chartX;
            mouseTracker.mouseDownY = e.chartY;
            addEvent(doc, 'mouseup', drop);
            if (hasTouch) {
                addEvent(doc, 'touchend', drop);
            }
        };
        // The mousemove, touchmove and touchstart event handler
        let mouseMove = function (e) {
            // let the system handle multitouch operations like two finger scroll
            // and pinching
            if (e && e.touches && e.touches.length > 1) {
                return;
            }
            // normalize
            e = mouseTracker.normalizeMouseEvent(e);
            let type = e.type,
                chartX = e.chartX,
                chartY = e.chartY,
                isOutsidePlot = !chart.isInsidePlot(chartX - chart.plotLeft, chartY - UI_CHART_PLOT_TOP);
            if (type.indexOf('touch') === -1) {  // not for touch actions
                e.returnValue = false;
            }
            // on touch devices, only trigger click if a handler is defined
            if (type === 'touchstart') {
                if (attr(e.target, 'isTracker')) {
                    if (!chart.runTrackerClick) {
                        e.preventDefault();
                    }
                } else if (!isOutsidePlot) {
                    e.preventDefault();
                }
            }
            // cancel on mouse outside
            if (isOutsidePlot) {
                /*if (!lastWasOutsidePlot) {
                    // reset the tracker
                    resetTracker();
                }*/
                // drop the selection if any and reset mouseIsDown and hasDragged
                //drop();
                if (chartX < chart.plotLeft) {
                    chartX = chart.plotLeft;
                } else if (chartX > chart.plotLeft + chart.plotWidth) {
                    chartX = chart.plotLeft + chart.plotWidth;
                }
                if (chartY < UI_CHART_PLOT_TOP) {
                    chartY = UI_CHART_PLOT_TOP;
                } else if (chartY > UI_CHART_PLOT_TOP + chart.plotHeight) {
                    chartY = UI_CHART_PLOT_TOP + chart.plotHeight;
                }
            }
            if (chart.mouseIsDown && type !== 'touchstart') { // make selection
                // determine if the mouse has moved more than 10px
                hasDragged = Math.sqrt(
                    Math.pow(mouseTracker.mouseDownX - chartX, 2) +
                    Math.pow(mouseTracker.mouseDownY - chartY, 2)
                );
                if (hasDragged > 10) {
                    let clickedInside = chart.isInsidePlot(mouseTracker.mouseDownX - chart.plotLeft, mouseTracker.mouseDownY - UI_CHART_PLOT_TOP);
                    // make a selection
                    if (chart.hasCartesianSeries && (mouseTracker.zoomX || mouseTracker.zoomY) && clickedInside) {
                        if (!mouseTracker.selectionMarker) {
                            mouseTracker.selectionMarker = chart.renderer.rect(
                                chart.plotLeft,
                                UI_CHART_PLOT_TOP,
                                zoomHor ? 1 : chart.plotWidth,
                                zoomVert ? 1 : chart.plotHeight,
                                0
                            )
                            .attr({
                                fill: mouseTracker.options.chart.selectionMarkerFill || 'rgba(69,114,167,0.25)',
                                zIndex: 7
                            })
                            .add();
                        }
                    }
                    // adjust the width of the selection marker
                    if (mouseTracker.selectionMarker && zoomHor) {
                        let xSize = chartX - mouseTracker.mouseDownX;
                        mouseTracker.selectionMarker.attr({
                            width: mathAbs(xSize),
                            x: (xSize > 0 ? 0 : xSize) + mouseTracker.mouseDownX
                        });
                    }
                    // adjust the height of the selection marker
                    if (mouseTracker.selectionMarker && zoomVert) {
                        let ySize = chartY - mouseTracker.mouseDownY;
                        mouseTracker.selectionMarker.attr({
                            height: mathAbs(ySize),
                            y: (ySize > 0 ? 0 : ySize) + mouseTracker.mouseDownY
                        });
                    }
                    // panning
                    if (clickedInside && !mouseTracker.selectionMarker && mouseTracker.options.chart.panning) {
                        chart.pan(chartX);
                    }
                }
            }
            // Show the tooltip and run mouse over events (#977)
            if (!isOutsidePlot) {
                mouseTracker.onmousemove(e);
            }
            lastWasOutsidePlot = isOutsidePlot;
            // when outside plot, allow touch-drag by returning true
            return isOutsidePlot || !chart.hasCartesianSeries;
        };
        // When the mouse enters the container, run mouseMove
        if (!/Android 4\.0/.test(userAgent)) { // This hurts. Best effort for #1385.
            container.onmousemove = mouseMove;
        }
        /*
         * When the mouse leaves the container, hide the tracking (tooltip).
         */
        addEvent(container, 'mouseleave', mouseTracker.hideTooltipOnMouseLeave);
        // issue #149 workaround
        // The mouseleave event above does not always fire. Whenever the mouse is moving
        // outside the plotarea, hide the tooltip
        if (!hasTouch) { // #1385
            addEvent(doc, 'mousemove', mouseTracker.hideTooltipOnMouseMove);
        }
        container.ontouchstart = function (e) {
            // For touch devices, use touchmove to zoom
            if (mouseTracker.zoomX || mouseTracker.zoomY) {
                container.onmousedown(e);
            }
            // Show tooltip and prevent the lower mouse pseudo event
            mouseMove(e);
        };
        /*
         * Allow dragging the finger over the chart to read the values on touch
         * devices
         */
        container.ontouchmove = mouseMove;
        /*
         * Allow dragging the finger over the chart to read the values on touch
         * devices
         */
        container.ontouchend = function () {
            if (hasDragged) {
                mouseTracker.resetTracker();
            }
        };
        // MooTools 1.2.3 doesn't fire this in IE when using addEvent
        container.onclick = function (e) {
            let hoverPoint = chart.hoverPoint,
                plotX,
                plotY;
            e = mouseTracker.normalizeMouseEvent(e);
            if (!chart.cancelClick) {
                // Detect clicks on trackers or tracker groups, #783
                if (hoverPoint && (attr(e.target, 'isTracker') || attr(e.target.parentNode, 'isTracker'))) {
                    plotX = hoverPoint.plotX;
                    plotY = hoverPoint.plotY;
                    // add page position info
                    extend(hoverPoint, {
                        pageX: mouseTracker.chartPosition.left + chart.plotLeft +
                            (plotX),
                        pageY: mouseTracker.chartPosition.top + UI_CHART_PLOT_TOP +
                            (plotY)
                    });
                    // the series click event
                    fireEvent(hoverPoint.seriesList, 'click', extend(e, {
                        point: hoverPoint
                    }));
                    // the point click event
                    hoverPoint.firePointEvent('click', e);
                } else {
                    extend(e, mouseTracker.getMouseCoordinates(e));
                    // fire a click event in the chart
                    if (chart.isInsidePlot(e.chartX - chart.plotLeft, e.chartY - UI_CHART_PLOT_TOP)) {
                        fireEvent(chart, 'click', e);
                    }
                }
            }
        };
    },
    /**
     * Destroys the MouseTracker object and disconnects DOM events.
     */
    destroy: function () {
        let mouseTracker = this,
            chart = mouseTracker.chart,
            container = chart.container;
        // Destroy the tracker group element
        if (chart.trackerGroup) {
            chart.trackerGroup = chart.trackerGroup.destroy();
        }
        removeEvent(container, 'mouseleave', mouseTracker.hideTooltipOnMouseLeave);
        removeEvent(doc, 'mousemove', mouseTracker.hideTooltipOnMouseMove);
        container.onclick = container.onmousedown = container.onmousemove = container.ontouchstart = container.ontouchend = container.ontouchmove = null;
        // memory and CPU leak
        clearInterval(this.tooltipTimeout);
    },
    // Run MouseTracker
    init: function (chart, options) {
        if (!chart.trackerGroup) {
            chart.trackerGroup = chart.renderer.g('tracker')
                .attr({ zIndex: 9 })
                .add();
        }
        if (options.enabled) {
            chart.tooltip = new Tooltip(chart, options);
        }
        this.setDOMEvents();
    }
};
/**
 * The overview of the chart's series
 */
/**
 * The chart class
 * @param {Object} options
 * @param {Function} callback Function to run when the chart has loaded
 */
function Chart() {
    this.init.apply(this, arguments);
}
Chart.prototype = {
    /**
     * Initialize the chart
     */
    init: function (userOptions, callback) {
        // Handle regular options
        let options,
            seriesOptions = userOptions.seriesList; // skip merging data points to increase performance
        userOptions.seriesList = null;
        options = merge(defaultOptions, userOptions); // do the merge
        options.seriesList = userOptions.seriesList = seriesOptions; // set back the series data
        let optionsChart = options.chart,
            optionsMargin = optionsChart.margin,
            margin = isObject(optionsMargin) ?
                optionsMargin :
                [optionsMargin, optionsMargin, optionsMargin, optionsMargin];
        this.optionsMarginTop = pick(optionsChart.marginTop, margin[0]);
        this.optionsMarginLeft = pick(optionsChart.marginLeft, margin[3]);
        this.callback = callback;
        this.isResizing = 0;
        this.options = options;
        this.axisList = [];
        this.seriesList = [];
        this.hasCartesianSeries = optionsChart.showAxes;
        //this.axisOffset = UNDEFINED;
        //this.maxTicks = UNDEFINED; // handle the greatest amount of ticks on grouped axes
        //this.loadingShown = UNDEFINED;
        //this.container = UNDEFINED;
        //this.chartWidth = UNDEFINED;
        //this.chartHeight = UNDEFINED;
        //this.containerWidth = UNDEFINED;
        //this.containerHeight = UNDEFINED;
        //this.oldChartWidth = UNDEFINED;
        //this.renderTo = UNDEFINED;
        //this.renderToClone = UNDEFINED;
        //this.tracker = UNDEFINED;
        //this.spacingBox = UNDEFINED
        //this.legend = UNDEFINED;
        // Elements
        //this.chartBackground = UNDEFINED;
        //this.plotBGImage = UNDEFINED;
        //this.plotBorder = UNDEFINED;
        //this.loadingDiv = UNDEFINED;
        //this.loadingSpan = UNDEFINED;
        let chart = this,
            eventType;
        // Add the chart to the global lookup
        chart.index = charts.length;
        charts.push(chart);
        chart.xAxis = [];
        chart.yAxis = [];
        // Expose methods and variables
        chart.animation = pick(optionsChart.animation, true);
        chart.pointCount = 0;
        chart.counters = new ChartCounters();
        chart.firstRender();
    },
    /**
     * Initialize an individual series, called internally before render time
     */
    initSeries: function (options) {
        let chart = this,
            optionsChart = chart.options.chart,
            type = options.type || optionsChart.type || optionsChart.defaultSeriesType,
            series = new seriesTypes[type]();
        series.init(this, options);
        return series;
    },
    /**
     * Check whether a given point is within the plot area
     *
     * @param {Number} plotX Pixel x relative to the plot area
     * @param {Number} plotY Pixel y relative to the plot area
     * @param {Boolean} inverted Whether the chart is inverted
     */
    isInsidePlot: function (plotX, plotY) {
        let x = plotX,
            y = plotY;
        return x >= 0 &&
            x <= this.plotWidth &&
            y >= 0 &&
            y <= this.plotHeight;
    },
    /**
     * Adjust all axes tick amounts
     */
    adjustTickAmounts: function () {
        if (this.options.chart.alignTicks !== false) {
            each(this.axisList, function (axis) {
                axis.adjustTickAmount();
            });
        }
        this.maxTicks = null;
    },
    /**
     * Redraw legend, axes or series based on updated data
     *
     * @param {Boolean|Object} animation Whether to apply animation, and optionally animation
     *    configuration
     */
    redraw: function (animation) {
        let chart = this,
            axes = chart.axisList,
            series = chart.seriesList,
            tracker = chart.tracker,
            isDirtyBox = chart.isDirtyBox, // todo: check if it has actually changed?
            seriesLength = series.length,
            i = seriesLength,
            serie,
            renderer = chart.renderer,
            isHiddenChart = renderer.isHidden(),
            afterRedraw = [];
        // handle added or removed series
        if (chart.hasCartesianSeries) {
            if (!chart.isResizing) {
                // reset maxTicks
                chart.maxTicks = null;
                // set axes scales
                each(axes, function (axis) {
                    axis.setScale();
                });
            }
            chart.adjustTickAmounts();
            chart.getMargins();
            // redraw axes
            each(axes, function (axis) {
                // Fire 'afterSetExtremes' only if extremes are set
                if (axis.isDirtyExtremes) { // #821
                    axis.isDirtyExtremes = false;
                    afterRedraw.push(function () { // prevent a recursive call to chart.redraw() (#1119)
                        fireEvent(axis, 'afterSetExtremes', axis.getExtremes()); // #747, #751
                    });
                }
                if (axis.isDirty || isDirtyBox) {
                    axis.redraw();
                    isDirtyBox = true; // #792
                }
            });
        }
        // the plot areas size has changed
        if (isDirtyBox) {
            chart.drawChartBox();
        }
        // redraw affected series
        each(series, function (serie) {
            if (serie.isDirty && serie.visible &&
                    (!serie.isCartesian || serie.xAxis)) { // issue #153
                serie.redraw();
            }
        });
        // move tooltip or reset
        if (tracker && tracker.resetTracker) {
            tracker.resetTracker(true);
        }
        // redraw if canvas
        renderer.draw();
        // fire the event
        fireEvent(chart, 'redraw'); // jQuery breaks this when calling it from addEvent. Overwrites chart.redraw
        // Fire callbacks that are put on hold until after the redraw
        each(afterRedraw, function (callback) {
            callback.call();
        });
    },
    /**
     * Create the Axis instances based on the config options
     */
    getAxes: function () {
        let chart = this,
            options = this.options;
        let xAxisOptions = options.xAxis || {},
            yAxisOptions = options.yAxis || {},
            optionsArray,
            axis;
        // make sure the options are arrays and add some members
        xAxisOptions = splat(xAxisOptions);
        each(xAxisOptions, function (axis, i) {
            axis.index = i;
            axis.isX = true;
        });
        yAxisOptions = splat(yAxisOptions);
        each(yAxisOptions, function (axis, i) {
            axis.index = i;
        });
        // concatenate all axis options into one array
        optionsArray = xAxisOptions.concat(yAxisOptions);
        each(optionsArray, function (axisOptions) {
            axis = new Axis(chart, axisOptions);
        });
        chart.adjustTickAmounts();
    },
    /**
     * Get the currently selected points from all series
     */
    getSelectedPoints: function () {
        let points = [];
        each(this.seriesList, function (serie) {
            points = points.concat(grep(serie.points, function (point) {
                return point.selected;
            }));
        });
        return points;
    },
    /**
     * Get the currently selected series
     */
    getSelectedSeries: function () {
        return grep(this.seriesList, function (serie) {
            return serie.selected;
        });
    },
    /**
     * Zoom out to 1:1
     */
    zoomOut: function () {
        let chart = this;
        fireEvent(chart, 'selection', { resetSelection: true }, function () { chart.zoom(); });
    },
    /**
     * Zoom into a given portion of the chart given by axis coordinates
     * @param {Object} event
     */
    zoom: function (event) {
        let chart = this,
            hasZoomed;
        // if zoom is called with no arguments, reset the axes
        if (!event || event.resetSelection) {
            each(chart.axisList, function (axis) {
                hasZoomed = axis.zoom();
            });
        } else { // else, zoom in on all axes
            each(event.xAxis.concat(event.yAxis), function (axisData) {
                let axis = axisData.axis;
                // don't zoom more than minRange
                if (chart.tracker[axis.isXAxis ? 'zoomX' : 'zoomY']) {
                    hasZoomed = axis.zoom(axisData.min, axisData.max);
                }
            });
        }
        // Redraw
        if (hasZoomed) {
            chart.redraw(
                pick(chart.options.chart.animation, chart.pointCount < 100) // animation
            );
        }
    },
    /**
     * Pan the chart by dragging the mouse across the pane. This function is called
     * on mouse move, and the distance to pan is computed from chartX compared to
     * the first chartX position in the dragging operation.
     */
    pan: function (chartX) {
        let chart = this;
        let xAxis = chart.xAxis[0],
            mouseDownX = chart.mouseDownX,
            halfPointRange = xAxis.pointRange / 2,
            extremes = xAxis.getExtremes(),
            newMin = xAxis.translate(mouseDownX - chartX, true) + halfPointRange,
            newMax = xAxis.translate(mouseDownX + chart.plotWidth - chartX, true) - halfPointRange,
            hoverPoints = chart.hoverPoints;
        // remove active points for shared tooltip
        if (hoverPoints) {
            each(hoverPoints, function (point) {
                point.setState();
            });
        }
        if (xAxis.seriesList.length && newMin > mathMin(extremes.dataMin, extremes.min) && newMax < mathMax(extremes.dataMax, extremes.max)) {
            xAxis.setExtremes(newMin, newMax, true, false, { trigger: 'pan' });
        }
        chart.mouseDownX = chartX; // set new reference for next run
        css(chart.container, { cursor: 'move' });
    },
    /**
     * Get chart width and height according to options and container size
     */
    getChartSize: function () {
        let chart = this,
            optionsChart = chart.options.chart,
            renderTo = chart.renderToClone || chart.renderTo;
        // get inner width and height from jQuery (#824)
        chart.containerWidth = adapterRun(renderTo, 'width');
        chart.containerHeight = adapterRun(renderTo, 'height');
        chart.chartWidth = mathMax(0, pick(optionsChart.width, chart.containerWidth, 600));
        chart.chartHeight = mathMax(0, pick(optionsChart.height,
            // the offsetHeight of an empty container is 0 in standard browsers, but 19 in IE7:
            chart.containerHeight > 19 ? chart.containerHeight : 400));
    },
    /**
     * Get the containing element, determine the size and create the inner container
     * div to hold the chart
     */
    getContainer: function () {
        let chart = this,
            container,
            optionsChart = chart.options.chart,
            chartWidth,
            chartHeight,
            renderTo,
            indexAttrName = 'data-highcharts-chart',
            oldChartIndex,
            containerId;
        chart.renderTo = renderTo = optionsChart.renderTo;
        containerId = PREFIX + idCounter++;
        if (isString(renderTo)) {
            chart.renderTo = renderTo = doc.getElementById(renderTo);
        }
        // Display an error if the renderTo is wrong
        if (!renderTo) {
            error(13, true);
        }
        // If the container already holds a chart, destroy it
        oldChartIndex = pInt(attr(renderTo, indexAttrName));
        if (!isNaN(oldChartIndex) && charts[oldChartIndex]) {
            charts[oldChartIndex].destroy();
        }
        // Make a reference to the chart from the div
        attr(renderTo, indexAttrName, chart.index);
        // remove previous chart
        renderTo.innerHTML = '';
        // get the width and height
        chart.getChartSize();
        chartWidth = chart.chartWidth;
        chartHeight = chart.chartHeight;
        // create the inner container
        chart.container = container = createElement(DIV, {
                className: PREFIX + 'container' +
                    (optionsChart.className ? ' ' + optionsChart.className : ''),
                id: containerId
            }, extend({
                position: RELATIVE,
                overflow: HIDDEN, // needed for context menu (avoid scrollbars) and
                    // content overflow in IE
                width: chartWidth + 'px',
                height: chartHeight + 'px',
                textAlign: 'left',
                lineHeight: 'normal', // #427
                zIndex: 0 // #1072
            }, optionsChart.style),
            chart.renderToClone || renderTo
        );
        chart.renderer =
                new SVGRenderer(container, chartWidth, chartHeight, true);
    },
    /**
     * Calculate margins by rendering axis labels in a preliminary position. Title,
     * subtitle and legend have already been rendered at this stage, but will be
     * moved into their final positions
     */
    getMargins: function () {
        let chart = this,
            optionsChart = chart.options.chart,
            spacingTop = optionsChart.spacingTop,
            spacingRight = optionsChart.spacingRight,
            spacingBottom = optionsChart.spacingBottom,
            spacingLeft = optionsChart.spacingLeft,
            axisOffset,
            optionsMarginTop = chart.optionsMarginTop,
            optionsMarginLeft = chart.optionsMarginLeft;
        chart.resetMargins();
        axisOffset = chart.axisOffset;
        // pre-render axes to get labels offset width
        if (chart.hasCartesianSeries) {
            each(chart.axisList, function (axis) {
                axis.getOffset();
            });
        }
        if (!defined(optionsMarginLeft)) {
            chart.plotLeft += axisOffset[3];
        }
        chart.setChartSize();
    },
    /**
     * Set the public chart properties. This is done before and after the pre-render
     * to determine margin sizes
     */
    setChartSize: function () {
        let chart = this,
            chartWidth = chart.chartWidth,
            chartHeight = chart.chartHeight,
            optionsChart = chart.options.chart,
            spacingTop = optionsChart.spacingTop,
            spacingRight = optionsChart.spacingRight,
            spacingBottom = optionsChart.spacingBottom,
            spacingLeft = optionsChart.spacingLeft,
            plotLeft,
            plotWidth,
            plotHeight,
            plotBorderWidth;
        chart.plotLeft = plotLeft = mathRound(chart.plotLeft);
        chart.plotWidth = plotWidth = mathMax(0, mathRound(chartWidth - plotLeft - UI_CHART_MARGIN_RIGHT));
        chart.plotHeight = plotHeight = mathMax(0, mathRound(chartHeight - UI_CHART_PLOT_TOP - UI_CHART_MARGIN_BOTTOM));
        chart.plotSizeX = plotWidth;
        chart.plotSizeY = plotHeight;
        chart.plotBorderWidth = plotBorderWidth = optionsChart.plotBorderWidth || 0;
        // Set boxes used for alignment
        chart.spacingBox = {
            x: spacingLeft,
            y: spacingTop,
            width: chartWidth - spacingLeft - spacingRight,
            height: chartHeight - spacingTop - spacingBottom
        };
        chart.plotBox = {
            x: plotLeft,
            y: UI_CHART_PLOT_TOP,
            width: plotWidth,
            height: plotHeight
        };
        chart.clipBox = {
            x: plotBorderWidth / 2,
            y: plotBorderWidth / 2,
            width: chart.plotSizeX - plotBorderWidth,
            height: chart.plotSizeY - plotBorderWidth
        };
        each(chart.axisList, function (axis) {
            axis.setAxisSize();
            axis.setAxisTranslation();
        });
    },
    /**
     * Initial margins before auto size margins are applied
     */
    resetMargins: function () {
        let chart = this,
            optionsChart = chart.options.chart,
            spacingTop = optionsChart.spacingTop,
            spacingRight = optionsChart.spacingRight,
            spacingBottom = optionsChart.spacingBottom,
            spacingLeft = optionsChart.spacingLeft;
        chart.plotLeft = pick(chart.optionsMarginLeft, spacingLeft);
        chart.axisOffset = [0, 0, 0, 0]; // top, right, bottom, left
    },
    /**
     * Draw the borders and backgrounds for chart and plot area
     */
    drawChartBox: function () {
        let chart = this,
            optionsChart = chart.options.chart,
            renderer = chart.renderer,
            chartWidth = chart.chartWidth,
            chartHeight = chart.chartHeight,
            chartBackground = chart.chartBackground,
            plotBorder = chart.plotBorder,
            plotBGImage = chart.plotBGImage,
            chartBorderWidth = optionsChart.borderWidth || 0,
            chartBackgroundColor = optionsChart.backgroundColor,
            plotBorderWidth = optionsChart.plotBorderWidth || 0,
            mgn,
            bgAttr,
            plotLeft = chart.plotLeft,
            plotWidth = chart.plotWidth,
            plotHeight = chart.plotHeight,
            plotBox = chart.plotBox,
            clipRect = chart.clipRect,
            clipBox = chart.clipBox;
        // Chart area
        mgn = chartBorderWidth + (optionsChart.shadow ? 8 : 0);
        if (chartBorderWidth || chartBackgroundColor) {
            if (!chartBackground) {
                bgAttr = {
                    fill: chartBackgroundColor || NONE
                };
                if (chartBorderWidth) { // #980
                    bgAttr.stroke = optionsChart.borderColor;
                    bgAttr['stroke-width'] = chartBorderWidth;
                }
                chart.chartBackground = renderer.rect(mgn / 2, mgn / 2, chartWidth - mgn, chartHeight - mgn,
                        optionsChart.borderRadius, chartBorderWidth)
                    .attr(bgAttr)
                    .add()
                    .shadow(optionsChart.shadow);
            } else { // resize
                chartBackground.animate(
                    chartBackground.crisp(null, null, null, chartWidth - mgn, chartHeight - mgn)
                );
            }
        }
        // Plot clip
        if (!clipRect) {
            chart.clipRect = renderer.clipRect(clipBox);
        } else {
            clipRect.animate({
                width: clipBox.width,
                height: clipBox.height
            });
        }
        // Plot area border
        if (plotBorderWidth) {
            if (!plotBorder) {
                chart.plotBorder = renderer.rect(plotLeft, UI_CHART_PLOT_TOP, plotWidth, plotHeight, 0, plotBorderWidth)
                    .attr({
                        stroke: optionsChart.plotBorderColor,
                        'stroke-width': plotBorderWidth,
                        zIndex: 1
                    })
                    .add();
            } else {
                plotBorder.animate(
                    plotBorder.crisp(null, plotLeft, UI_CHART_PLOT_TOP, plotWidth, plotHeight)
                );
            }
        }
        // reset
        chart.isDirtyBox = false;
    },
    /**
     * Render all graphics for the chart
     */
    render: function () {
        let chart = this,
            axes = chart.axisList,
            renderer = chart.renderer,
            options = chart.options;
        let labels = options.labels;
        // Get margins by pre-rendering axes
        // set axes scales
        each(axes, function (axis) {
            axis.setScale();
        });
        chart.getMargins();
        chart.maxTicks = null; // reset for second pass
        each(axes, function (axis) {
            axis.setTickPositions(true); // update to reflect the new margins
            axis.setMaxTicks();
        });
        chart.adjustTickAmounts();
        chart.getMargins(); // second pass to check for new labels
        // Draw the borders and backgrounds
        chart.drawChartBox();
        // Axes
        if (chart.hasCartesianSeries) {
            each(axes, function (axis) {
                axis.render();
            });
        }
        // The series
        if (!chart.seriesGroup) {
            chart.seriesGroup = renderer.g('series-group')
                .attr({ zIndex: 3 })
                .add();
        }
        each(chart.seriesList, function (serie) {
            serie.translate();
            serie.setTooltipPoints();
            serie.render();
        });
        // Labels
        if (labels.items) {
            each(labels.items, function (label) {
                let style = extend(labels.style, label.style),
                    x = pInt(style.left) + chart.plotLeft,
                    y = pInt(style.top) + UI_CHART_PLOT_TOP + 12;
                // delete to prevent rewriting in IE
                delete style.left;
                delete style.top;
                renderer.text(
                    label.html,
                    x,
                    y
                )
                .attr({ zIndex: 2 })
                .css(style)
                .add();
            });
        }
        // Set flag
        chart.hasRendered = true;
    },
    /**
     * Clean up memory usage
     */
    destroy: function () {
        let chart = this,
            axes = chart.axisList,
            series = chart.seriesList,
            container = chart.container,
            i,
            parentNode = container && container.parentNode;
        // fire the chart.destoy event
        fireEvent(chart, 'destroy');
        // Delete the chart from charts lookup array
        charts[chart.index] = UNDEFINED;
        chart.renderTo.removeAttribute('data-highcharts-chart');
        // remove events
        removeEvent(chart);
        // ==== Destroy collections:
        // Destroy axes
        i = axes.length;
        while (i--) {
            axes[i] = axes[i].destroy();
        }
        // Destroy each series
        i = series.length;
        while (i--) {
            series[i] = series[i].destroy();
        }
        // ==== Destroy chart properties:
        each(['chartBackground', 'plotBackground', 'plotBGImage',
                'plotBorder', 'seriesGroup', 'clipRect', 'tracker', 'scroller',
                'rangeSelector', 'tooltip', 'renderer'], function (name) {
            let prop = chart[name];
            if (prop && prop.destroy) {
                chart[name] = prop.destroy();
            }
        });
        // remove container and all SVG
        if (container) { // can break in IE when destroyed before finished loading
            container.innerHTML = '';
            removeEvent(container);
            if (parentNode) {
                discardElement(container);
            }
        }
        // clean it all up
        for (i in chart) {
            delete chart[i];
        }
    },
    /**
     * Prepare for first rendering after all data are loaded
     */
    firstRender: function () {
        let chart = this,
            options = chart.options,
            callback = chart.callback;
        // Create the container
        chart.getContainer();
        // Run an early event after the container and renderer are established
        fireEvent(chart, 'init');
        // Initialize range selector for stock charts
        if (Highcharts.RangeSelector && options.rangeSelector.enabled) {
            chart.rangeSelector = new Highcharts.RangeSelector(chart);
        }
        chart.resetMargins();
        chart.setChartSize();
        // get axes
        chart.getAxes();
        // Initialize the series
        each(options.seriesList || [], function (serieOptions) {
            chart.initSeries(serieOptions);
        });
        // Run an event where series and axes can be added
        //fireEvent(chart, 'beforeRender');
        // Initialize scroller for stock charts
        if (Highcharts.Scroller && (options.navigator.enabled || options.scrollbar.enabled)) {
            chart.scroller = new Highcharts.Scroller(chart);
        }
        // depends on inverted and on margins being set
        chart.tracker = new MouseTracker(chart, options);
        chart.render();
        // add canvas
        chart.renderer.draw();
        // run callbacks
        if (callback) {
            callback.apply(chart, [chart]);
        }
        each(chart.callbacks, function (fn) {
            fn.apply(chart, [chart]);
        });
        fireEvent(chart, 'load');
    }
}; // end Chart
// Hook for exporting module
Chart.prototype.callbacks = [];
/**
 * The Point object and prototype. Inheritable and used as base for PiePoint
 */
let Point = function () {};
Point.prototype = {
    /**
     * Initialize the point
     * @param {Object} series The series object containing this point
     * @param {Object} options The data in either number, array or object format
     */
    init: function (series, options, x) {
        let point = this,
            counters = series.chart.counters,
            defaultColors;
        point.seriesList = series;
        point.applyOptions(options, x);
        point.pointAttr = {};
        if (series.options.colorByPoint) {
            defaultColors = series.chart.options.colors;
            point.color = point.color || defaultColors[counters.color++];
            // loop back to zero
            counters.wrapColor(defaultColors.length);
        }
        series.chart.pointCount++;
        return point;
    },
    /**
     * Apply the options containing the x and y data and possible some extra properties.
     * This is called on point init or from point.update.
     *
     * @param {Object} options
     */
    applyOptions: function (options, x) {
        let point = this,
            series = point.seriesList,
            optionsType = typeof options;
        point.config = options;
        // onedimensional array input
        if (optionsType === 'number' || options === null) {
            point.y = options;
        } else if (typeof options[0] === 'number') { // two-dimentional array
            point.x = options[0];
            point.y = options[1];
        } else if (optionsType === 'object' && typeof options.length !== 'number') { // object input
            // copy options directly to point
            extend(point, options);
            point.options = options;
            // This is the fastest way to detect if there are individual point dataLabels that need
            // Same approach as above for markers
            if (options.marker) {
                series._hasPointMarkers = true;
            }
        } else if (typeof options[0] === 'string') { // categorized data with name in first position
            point.name = options[0];
            point.y = options[1];
        }
        /*
         * If no x is set by now, get auto incremented value. All points must have an
         * x value, however the y value can be null to create a gap in the series
         */
        // todo: skip this? It is only used in applyOptions, in translate it should not be used
        if (point.x === UNDEFINED) {
            point.x = x === UNDEFINED ? series.autoIncrement() : x;
        }
    },
    /**
     * Destroy a point to clear memory. Its reference still stays in series.data.
     */
    destroy: function () {
        let point = this,
            series = point.seriesList,
            chart = series.chart,
            hoverPoints = chart.hoverPoints,
            prop;
        chart.pointCount--;
        if (hoverPoints) {
            point.setState();
            erase(hoverPoints, point);
            if (!hoverPoints.length) {
                chart.hoverPoints = null;
            }
        }
        if (point === chart.hoverPoint) {
            point.onMouseOut();
        }
        // remove all events
        if (point.graphic || point.dataLabel) { // removeEvent and destroyElements are performance expensive
            removeEvent(point);
            point.destroyElements();
        }
        for (prop in point) {
            point[prop] = null;
        }
    },
    /**
     * Destroy SVG elements associated with the point
     */
    destroyElements: function () {
        let point = this,
            props = ['graphic', 'tracker', 'dataLabel', 'dataLabelUpper', 'group', 'connector', 'shadowGroup'],
            prop,
            i = 6;
        while (i--) {
            prop = props[i];
            if (point[prop]) {
                point[prop] = point[prop].destroy();
            }
        }
    },
    /**
     * Return the configuration hash needed for the data label and tooltip formatters
     */
    getLabelConfig: function () {
        let point = this;
        return {
            x: point.category,
            y: point.y,
            key: point.name || point.category,
            seriesList: point.seriesList,
            point: point,
            percentage: point.percentage,
            total: point.total
        };
    },
    onMouseOver: function () {
        let point = this,
            series = point.seriesList,
            chart = series.chart,
            tooltip = chart.tooltip,
            hoverPoint = chart.hoverPoint;
        // set normal state to previous series
        if (hoverPoint && hoverPoint !== point) {
            hoverPoint.onMouseOut();
        }
        // trigger the event
        point.firePointEvent('mouseOver');
        // update the tooltip
        if (tooltip) {
            tooltip.refresh(point);
        }
        // hover this
        point.setState(HOVER_STATE);
        chart.hoverPoint = point;
    },
    onMouseOut: function () {
        let chart = this.seriesList.chart,
            hoverPoints = chart.hoverPoints;
        if (!hoverPoints || inArray(this, hoverPoints) === -1) { // #887
            this.firePointEvent('mouseOut');
            this.setState();
            chart.hoverPoint = null;
        }
    },
    /**
     * Extendable method for formatting each point's tooltip line
     *
     * @return {String} A string to be concatenated in to the common tooltip text
     */
    tooltipFormatter: function (pointFormat) {
        let point = this,
            series = point.seriesList,
            seriesTooltipOptions = series.tooltipOptions,
            match = pointFormat.match(/\{(series|point)\.[a-zA-Z]+\}/g),
            splitter = /[{\.}]/,
            obj,
            key,
            replacement,
            repOptionKey,
            parts,
            prop,
            i,
            cfg = {
                y: 0, // 0: use 'value' for repOptionKey
                open: 0,
                high: 0,
                low: 0,
                close: 0,
                percentage: 1, // 1: use the self name for repOptionKey
                total: 1
            };
        // Backwards compatibility to y naming in early Highstock
        seriesTooltipOptions.valuePrefix = seriesTooltipOptions.valuePrefix || seriesTooltipOptions.yPrefix;
        seriesTooltipOptions.valueDecimals = pick(seriesTooltipOptions.valueDecimals, seriesTooltipOptions.yDecimals); // #1248
        seriesTooltipOptions.valueSuffix = seriesTooltipOptions.valueSuffix || seriesTooltipOptions.ySuffix;
        // loop over the variables defined on the form {series.name}, {point.y} etc
        for (i in match) {
            key = match[i];
            if (isString(key) && key !== pointFormat) { // IE matches more than just the variables
                // Split it further into parts
                parts = (' ' + key).split(splitter); // add empty string because IE and the rest handles it differently
                obj = { 'point': point, 'series': series }[parts[1]];
                prop = parts[2];
                // Add some preformatting
                if (obj === point && cfg.hasOwnProperty(prop)) {
                    repOptionKey = cfg[prop] ? prop : 'value';
                    replacement = (seriesTooltipOptions[repOptionKey + 'Prefix'] || '') +
                        numberFormat(point[prop], pick(seriesTooltipOptions[repOptionKey + 'Decimals'], -1)) +
                        (seriesTooltipOptions[repOptionKey + 'Suffix'] || '');
                // Automatic replacement
                } else {
                    replacement = obj[prop];
                }
                pointFormat = pointFormat.replace(key, replacement);
            }
        }
        return pointFormat;
    },
    /**
     * Fire an event on the Point object. Must not be renamed to fireEvent, as this
     * causes a name clash in MooTools
     * @param {String} eventType
     * @param {Object} eventArgs Additional event arguments
     * @param {Function} defaultFunction Default event handler
     */
    firePointEvent: function (eventType, eventArgs, defaultFunction) {
        let point = this,
            series = this.seriesList,
            seriesOptions = series.options;
        // add default handler if in selection mode
        if (eventType === 'click' && seriesOptions.allowPointSelect) {
            defaultFunction = function (event) {
                // Control key is for Windows, meta (= Cmd key) for Mac, Shift for Opera
                point.select(null, event.ctrlKey || event.metaKey || event.shiftKey);
            };
        }
        fireEvent(this, eventType, eventArgs, defaultFunction);
    },
    /**
     * Set the point's state
     * @param {String} state
     */
    setState: function (state) {
        let point = this,
            plotX = point.plotX,
            plotY = point.plotY,
            series = point.seriesList,
            stateOptions = series.options.states,
            markerOptions = defaultPlotOptions[series.type].marker && series.options.marker,
            normalDisabled = markerOptions && !markerOptions.enabled,
            markerStateOptions = markerOptions && markerOptions.states[state],
            stateDisabled = markerStateOptions && markerStateOptions.enabled === false,
            stateMarkerGraphic = series.stateMarkerGraphic,
            chart = series.chart,
            radius,
            pointAttr = point.pointAttr;
        state = state || NORMAL_STATE; // empty string
        if (
                // already has this state
                state === point.state ||
                // selected points don't respond to hover
                (point.selected && state !== SELECT_STATE) ||
                // series' state options is disabled
                (stateOptions[state] && stateOptions[state].enabled === false) ||
                // point marker's state options is disabled
                (state && (stateDisabled || (normalDisabled && !markerStateOptions.enabled)))
            ) {
            return;
        }
        // apply hover styles to the existing point
        if (point.graphic) {
            radius = markerOptions && point.graphic.symbolName && pointAttr[state].r;
            point.graphic.attr(merge(
                pointAttr[state],
                radius ? { // new symbol attributes (#507, #612)
                    x: plotX - radius,
                    y: plotY - radius,
                    width: 2 * radius,
                    height: 2 * radius
                } : {}
            ));
        } else {
            // if a graphic is not applied to each point in the normal state, create a shared
            // graphic for the hover state
            if (state && markerStateOptions) {
                radius = markerStateOptions.radius;
                if (!stateMarkerGraphic) { // add
                    series.stateMarkerGraphic = stateMarkerGraphic = chart.renderer.symbol(
                        series.symbol,
                        plotX - radius,
                        plotY - radius,
                        2 * radius,
                        2 * radius
                    )
                    .attr(pointAttr[state])
                    .add(series.markerGroup);
                } else { // update
                    stateMarkerGraphic.attr({ // #1054
                        x: plotX - radius,
                        y: plotY - radius
                    });
                }
            }
            if (stateMarkerGraphic) {
                stateMarkerGraphic[state && chart.isInsidePlot(plotX, plotY) ? 'show' : 'hide']();
            }
        }
        point.state = state;
    }
};
/**
 * @classDescription The base function which all other series types inherit from. The data in the series is stored
 * in various arrays.
 *
 * - First, series.options.data contains all the original config options for
 * each point whether added by options or methods like series.addPoint.
 * - Next, series.data contains those values converted to points, but in case the series data length
 * exceeds the cropThreshold, or if the data is grouped, series.data doesn't contain all the points. It
 * only contains the points that have been created on demand.
 * - Then there's series.points that contains all currently visible point objects. In case of cropping,
 * the cropped-away points are not part of this array. The series.points array starts at series.cropStart
 * compared to series.data and series.options.data. If however the series data is grouped, these can't
 * be correlated one to one.
 * - series.xData and series.processedXData contain clean x values, equivalent to series.data and series.points.
 * - series.yData and series.processedYData contain clean x values, equivalent to series.data and series.points.
 *
 * @param {Object} chart
 * @param {Object} options
 */
let Series = function () {};
Series.prototype = {
    isCartesian: true,
    type: 'line',
    pointClass: Point,
    sorted: true, // requires the data to be sorted
    requireSorting: true,
    pointAttrToOptions: { // mapping between SVG attributes and the corresponding options
        stroke: 'lineColor',
        'stroke-width': 'lineWidth',
        fill: 'fillColor',
        r: 'radius'
    },
    init: function (chart, options) {
        let series = this,
            eventType,
            events;
        series.chart = chart;
        series.options = options = series.setOptions(options); // merge with plotOptions
        // bind the axes
        series.bindAxes();
        // set some variables
        extend(series, {
            name: options.name,
            state: NORMAL_STATE,
            pointAttr: {},
            visible: options.visible !== false, // true by default
            selected: options.selected === true // false by default
        });
        series.getColor();
        series.getSymbol();
        // set the data
        series.setData(options.data, false);
        // Mark cartesian
        if (series.isCartesian) {
            chart.hasCartesianSeries = true;
        }
        // Register it in the chart
        chart.seriesList.push(series);
        // Sort series according to index option (#248, #1123)
        stableSort(chart.seriesList, function (a, b) {
            return (a.options.index || 0) - (b.options.index || 0);
        });
        each(chart.seriesList, function (series, i) {
            series.index = i;
            series.name = series.name || 'Series ' + (i + 1);
        });
    },
    /**
     * Set the xAxis and yAxis properties of cartesian series, and register the series
     * in the axis.seriesList array
     */
    bindAxes: function () {
        let series = this,
            seriesOptions = series.options,
            chart = series.chart,
            axisOptions;
        if (series.isCartesian) {
            each(['xAxis', 'yAxis'], function (AXIS) { // repeat for xAxis and yAxis
                each(chart[AXIS], function (axis) { // loop through the chart's axis objects
                    axisOptions = axis.options;
                    // apply if the series xAxis or yAxis option mathches the number of the
                    // axis, or if undefined, use the first axis
                    if ((seriesOptions[AXIS] === axisOptions.index) ||
                            (seriesOptions[AXIS] === UNDEFINED && axisOptions.index === 0)) {
                        // register this series in the axis.seriesList lookup
                        axis.seriesList.push(series);
                        // set this series.xAxis or series.yAxis reference
                        series[AXIS] = axis;
                        // mark dirty for redraw
                        axis.isDirty = true;
                    }
                });
            });
        }
    },
    /**
     * Return an auto incremented x value based on the pointStart and pointInterval options.
     * This is only used if an x value is not given for the point that calls autoIncrement.
     */
    autoIncrement: function () {
        let series = this,
            options = series.options,
            xIncrement = series.xIncrement;
        xIncrement = pick(xIncrement, options.pointStart, 0);
        series.pointInterval = pick(series.pointInterval, options.pointInterval, 1);
        series.xIncrement = xIncrement + series.pointInterval;
        return xIncrement;
    },
    /**
     * Divide the series data into segments divided by null values.
     */
    getSegments: function () {
        let series = this,
            lastNull = -1,
            segments = [],
            i,
            points = series.points,
            pointsLength = points.length;
        if (pointsLength) { // no action required for []
            // if connect nulls, just remove null points
            if (series.options.connectNulls) {
                i = pointsLength;
                while (i--) {
                    if (points[i].y === null) {
                        points.splice(i, 1);
                    }
                }
                if (points.length) {
                    segments = [points];
                }
            // else, split on null points
            } else {
                each(points, function (point, i) {
                    if (point.y === null) {
                        if (i > lastNull + 1) {
                            segments.push(points.slice(lastNull + 1, i));
                        }
                        lastNull = i;
                    } else if (i === pointsLength - 1) { // last value
                        segments.push(points.slice(lastNull + 1, i + 1));
                    }
                });
            }
        }
        // register it
        series.segments = segments;
    },
    /**
     * Set the series options by merging from the options tree
     * @param {Object} itemOptions
     */
    setOptions: function (itemOptions) {
        let chart = this.chart,
            chartOptions = chart.options,
            plotOptions = chartOptions.plotOptions,
            typeOptions = plotOptions[this.type],
            data = itemOptions.data,
            options;
        itemOptions.data = null; // remove from merge to prevent looping over the data set
        options = merge(
            typeOptions,
            plotOptions.seriesList,
            itemOptions
        );
        // Re-insert the data array to the options and the original config (#717)
        options.data = itemOptions.data = data;
        // the tooltip options are merged between global and series specific options
        this.tooltipOptions = merge(chartOptions.tooltip, options.tooltip);
        // Delte marker object if not allowed (#1125)
        if (typeOptions.marker === null) {
            delete options.marker;
        }
        return options;
    },
    /**
     * Get the series' color
     */
    getColor: function () {
        let options = this.options,
            defaultColors = this.chart.options.colors,
            counters = this.chart.counters;
        this.color = options.color ||
            (!options.colorByPoint && defaultColors[counters.color++]) || 'gray';
        counters.wrapColor(defaultColors.length);
    },
    /**
     * Get the series' symbol
     */
    getSymbol: function () {
        let series = this,
            seriesMarkerOption = series.options.marker,
            chart = series.chart,
            defaultSymbols = chart.options.symbols,
            counters = chart.counters;
        series.symbol = seriesMarkerOption.symbol || defaultSymbols[counters.symbol++];
        // don't substract radius in image symbols (#604)
        if (/^url/.test(series.symbol)) {
            seriesMarkerOption.radius = 0;
        }
        counters.wrapSymbol(defaultSymbols.length);
    },
    /**
     * Replace the series data with a new set of data
     * @param {Object} data
     * @param {Object} redraw
     */
    setData: function (data, redraw) {
        let series = this,
            oldData = series.points,
            options = series.options,
            initialColor = series.initialColor,
            chart = series.chart,
            firstPoint = null,
            xAxis = series.xAxis,
            i,
            pointProto = series.pointClass.prototype;
        // reset properties
        series.xIncrement = null;
        series.pointRange = xAxis && xAxis.categories ? 1 : options.pointRange;
        if (defined(initialColor)) { // reset colors for pie
            chart.counters.color = initialColor;
        }
        // parallel arrays
        let xData = [],
            yData = [],
            dataLength = data ? data.length : [],
            pt,
            pointArrayMap = series.pointArrayMap,
            valueCount = pointArrayMap && pointArrayMap.length;
        // In turbo mode, only one- or twodimensional arrays of numbers are allowed. The
        // first value is tested, and we assume that all the rest are defined the same
        // way. Although the 'for' loops are similar, they are repeated inside each
        // if-else conditional for max performance.
        // find the first non-null point
        i = 0;
        while (firstPoint === null && i < dataLength) {
            firstPoint = data[i];
            i++;
        }
        if (isNumber(firstPoint)) { // assume all points are numbers
            let x = pick(options.pointStart, 0),
                pointInterval = pick(options.pointInterval, 1);
            for (i = 0; i < dataLength; i++) {
                xData[i] = x;
                yData[i] = data[i];
                x += pointInterval;
            }
            series.xIncrement = x;
        } else if (isArray(firstPoint)) { // assume all points are arrays
            if (valueCount) { // [x, low, high] or [x, o, h, l, c]
                for (i = 0; i < dataLength; i++) {
                    pt = data[i];
                    xData[i] = pt[0];
                    yData[i] = pt.slice(1, valueCount + 1);
                }
            } else { // [x, y]
                for (i = 0; i < dataLength; i++) {
                    pt = data[i];
                    xData[i] = pt[0];
                    yData[i] = pt[1];
                }
            }
        } /* else {
                error(12); // Highcharts expects configs to be numbers or arrays in turbo mode
            }*/
        // Unsorted data is not supported by the line tooltip as well as data grouping and
        // navigation in Stock charts (#725)
        if (series.requireSorting && xData.length > 1 && xData[1] < xData[0]) {
            error(15);
        }
        // Forgetting to cast strings to numbers is a common caveat when handling CSV or JSON
        if (isString(yData[0])) {
            error(14, true);
        }
        series.data = [];
        series.options.data = data;
        series.xData = xData;
        series.yData = yData;
        // destroy old points
        i = (oldData && oldData.length) || 0;
        while (i--) {
            if (oldData[i] && oldData[i].destroy) {
                oldData[i].destroy();
            }
        }
        // reset minRange (#878)
        if (xAxis) {
            xAxis.minRange = xAxis.userMinRange;
        }
        // redraw
        series.isDirty = series.isDirtyData = chart.isDirtyBox = true;
        if (pick(redraw, true)) {
            chart.redraw(false);
        }
    },
    /**
     * Process the data by cropping away unused data points if the series is longer
     * than the crop threshold. This saves computing time for lage series.
     */
    processData: function (force) {
        let series = this,
            processedXData = series.xData, // copied during slice operation below
            processedYData = series.yData,
            dataLength = processedXData.length,
            cropStart = 0,
            cropEnd = dataLength,
            cropped,
            distance,
            closestPointRange,
            xAxis = series.xAxis,
            i, // loop variable
            options = series.options,
            cropThreshold = options.cropThreshold,
            isCartesian = series.isCartesian;
        // If the series data or axes haven't changed, don't go through this. Return false to pass
        // the message on to override methods like in data grouping.
        if (isCartesian && !series.isDirty && !xAxis.isDirty && !series.yAxis.isDirty && !force) {
            return false;
        }
        // optionally filter out points outside the plot area
        if (isCartesian && series.sorted && (!cropThreshold || dataLength > cropThreshold || series.forceCrop)) {
            let extremes = xAxis.getExtremes(),
                min = extremes.min,
                max = extremes.max;
            // it's outside current extremes
            if (processedXData[dataLength - 1] < min || processedXData[0] > max) {
                processedXData = [];
                processedYData = [];
            // only crop if it's actually spilling out
            } else if (processedXData[0] < min || processedXData[dataLength - 1] > max) {
                // iterate up to find slice start
                for (i = 0; i < dataLength; i++) {
                    if (processedXData[i] >= min) {
                        cropStart = mathMax(0, i - 1);
                        break;
                    }
                }
                // proceed to find slice end
                for (; i < dataLength; i++) {
                    if (processedXData[i] > max) {
                        cropEnd = i + 1;
                        break;
                    }
                }
                processedXData = processedXData.slice(cropStart, cropEnd);
                processedYData = processedYData.slice(cropStart, cropEnd);
                cropped = true;
            }
        }
        // Find the closest distance between processed points
        for (i = processedXData.length - 1; i > 0; i--) {
            distance = processedXData[i] - processedXData[i - 1];
            if (distance > 0 && (closestPointRange === UNDEFINED || distance < closestPointRange)) {
                closestPointRange = distance;
            }
        }
        // Record the properties
        series.cropped = cropped; // undefined or true
        series.cropStart = cropStart;
        series.processedXData = processedXData;
        series.processedYData = processedYData;
        if (options.pointRange === null) { // null means auto, as for columns, candlesticks and OHLC
            series.pointRange = closestPointRange || 1;
        }
        series.closestPointRange = closestPointRange;
    },
    /**
     * Generate the data point after the data has been processed by cropping away
     * unused points and optionally grouped in Highcharts Stock.
     */
    generatePoints: function () {
        let series = this,
            options = series.options,
            dataOptions = options.data,
            data = series.data,
            dataLength,
            processedXData = series.processedXData,
            processedYData = series.processedYData,
            pointClass = series.pointClass,
            processedDataLength = processedXData.length,
            cropStart = series.cropStart || 0,
            cursor,
            hasGroupedData = series.hasGroupedData,
            point,
            points = [],
            i;
        if (!data && !hasGroupedData) {
            let arr = [];
            arr.length = dataOptions.length;
            data = series.data = arr;
        }
        for (i = 0; i < processedDataLength; i++) {
            cursor = cropStart + i;
            if (!hasGroupedData) {
                if (data[cursor]) {
                    point = data[cursor];
                } else if (dataOptions[cursor] !== UNDEFINED) { // #970
                    data[cursor] = point = (new pointClass()).init(series, dataOptions[cursor], processedXData[i]);
                }
                points[i] = point;
            } else {
                // splat the y data in case of ohlc data array
                points[i] = (new pointClass()).init(series, [processedXData[i]].concat(splat(processedYData[i])));
            }
        }
        // Hide cropped-away points - this only runs when the number of points is above cropThreshold, or when
        // swithching view from non-grouped data to grouped data (#637)
        if (data && (processedDataLength !== (dataLength = data.length) || hasGroupedData)) {
            for (i = 0; i < dataLength; i++) {
                if (i === cropStart && !hasGroupedData) { // when has grouped data, clear all points
                    i += processedDataLength;
                }
                if (data[i]) {
                    data[i].destroyElements();
                    data[i].plotX = UNDEFINED; // #1003
                }
            }
        }
        series.data = data;
        series.points = points;
    },
    /**
     * Translate data points from raw data values to chart specific positioning data
     * needed later in drawPoints, drawGraph and drawTracker.
     */
    translate: function () {
        if (!this.processedXData) { // hidden series
            this.processData();
        }
        this.generatePoints();
        let series = this,
            chart = series.chart,
            options = series.options,
            xAxis = series.xAxis,
            categories = xAxis.categories,
            yAxis = series.yAxis,
            points = series.points,
            dataLength = points.length,
            hasModifyValue = !!series.modifyValue,
            isBottomSeries,
            placeBetween = options.pointPlacement === 'between';
            //nextSeriesDown;
        // Translate each point
        for (let i = 0; i < dataLength; i++) {
            let point = points[i],
                xValue = point.x,
                yValue = point.y,
                yBottom = point.low;
            // get the plotX translation
            //point.plotX = mathRound(xAxis.translate(xValue, 0, 0, 0, 1) * 10) / 10; // Math.round fixes #591
            point.plotX = xAxis.translate(xValue, 0, 0, 0, 1, placeBetween); // Math.round fixes #591
            // Set translated yBottom or remove it
            point.yBottom = defined(yBottom) ?
                yAxis.translate(yBottom, 0, 1, 0, 1) :
                null;
            // general hook, used for Highstock compare mode
            if (hasModifyValue) {
                yValue = series.modifyValue(yValue, point);
            }
            // Set the the plotY value, reset it for redraws
            point.plotY = (typeof yValue === 'number') ?
                mathRound(yAxis.translate(yValue, 0, 1, 0, 1) * 10) / 10 : // Math.round fixes #591
                UNDEFINED;
            // set client related positions for mouse tracking
            point.clientX = point.plotX; // for mouse tracking
            // some API data
            point.category = categories && categories[point.x] !== UNDEFINED ?
                categories[point.x] : point.x;
        }
        // now that we have the cropped data, build the segments
        series.getSegments();
    },
    /**
     * Memoize tooltip texts and positions
     */
    setTooltipPoints: function (renew) {
        let series = this,
            points = [],
            pointsLength,
            low,
            high,
            xAxis = series.xAxis,
            axisLength = xAxis ? (xAxis.tooltipLen || xAxis.len) : series.chart.plotSizeX, // tooltipLen and tooltipPosName used in polar
            plotX = (xAxis && xAxis.tooltipPosName) || 'plotX',
            point,
            i,
            tooltipPoints = []; // a lookup array for each pixel in the x dimension
        // don't waste resources if tracker is disabled
        if (series.options.enableMouseTracking === false) {
            return;
        }
        // renew
        if (renew) {
            series.tooltipPoints = null;
        }
        // concat segments to overcome null values
        each(series.segments || series.points, function (segment) {
            points = points.concat(segment);
        });
        // Reverse the points in case the X axis is reversed
        if (xAxis && xAxis.reversed) {
            points = points.reverse();
        }
        // Assign each pixel position to the nearest point
        pointsLength = points.length;
        for (i = 0; i < pointsLength; i++) {
            point = points[i];
            // Set this range's low to the last range's high plus one
            low = points[i - 1] ? high + 1 : 0;
            // Now find the new high
            high = points[i + 1] ?
                mathMax(0, mathFloor((point[plotX] + (points[i + 1] ? points[i + 1][plotX] : axisLength)) / 2)) :
                axisLength;
            while (low >= 0 && low <= high) {
                tooltipPoints[low++] = point;
            }
        }
        series.tooltipPoints = tooltipPoints;
    },
    /**
     * Format the header of the tooltip
     */
    tooltipHeaderFormatter: function (key) {
        let series = this,
            tooltipOptions = series.tooltipOptions,
            xDateFormat = tooltipOptions.xDateFormat,
            xAxis = series.xAxis,
            isDateTime = xAxis && xAxis.options.type === 'datetime',
            n;
        // Guess the best date format based on the closest point distance (#568)
        if (isDateTime && !xDateFormat) {
            for (n in timeUnits) {
                if (timeUnits[n] >= xAxis.closestPointRange) {
                    xDateFormat = tooltipOptions.dateTimeLabelFormats[n];
                    break;
                }
            }
        }
        return tooltipOptions.headerFormat
            .replace('{point.key}', isDateTime && isNumber(key) ? dateFormat(xDateFormat, key) :  key)
            .replace('{series.name}', series.name)
            .replace('{series.color}', series.color);
    },
    /**
     * Series mouse over handler
     */
    onMouseOver: function () {
        let series = this,
            chart = series.chart,
            hoverSeries = chart.hoverSeries;
        // set normal state to previous series
        if (hoverSeries && hoverSeries !== series) {
            hoverSeries.onMouseOut();
        }
        // trigger the event, but to save processing time,
        // only if defined
        if (series.options.events.mouseOver) {
            fireEvent(series, 'mouseOver');
        }
        // hover this
        series.setState(HOVER_STATE);
        chart.hoverSeries = series;
    },
    /**
     * Series mouse out handler
     */
    onMouseOut: function () {
        // trigger the event only if listeners exist
        let series = this,
            options = series.options,
            chart = series.chart,
            tooltip = chart.tooltip,
            hoverPoint = chart.hoverPoint;
        // trigger mouse out on the point, which must be in this series
        if (hoverPoint) {
            hoverPoint.onMouseOut();
        }
        // fire the mouse out event
        if (series && options.events.mouseOut) {
            fireEvent(series, 'mouseOut');
        }
        // hide the tooltip
        if (tooltip && !options.stickyTracking) {
            tooltip.hide();
        }
        // set normal state
        series.setState();
        chart.hoverSeries = null;
    },
    /**
     * Animate in the series
     */
    animate: function (init) {
        let series = this,
            chart = series.chart,
            renderer = chart.renderer,
            clipRect,
            markerClipRect,
            animation = series.options.animation,
            clipBox = chart.clipBox,
            sharedClipKey;
        // Animation option is set to true
        if (animation && !isObject(animation)) {
            animation = defaultPlotOptions[series.type].animation;
        }
        sharedClipKey = '_sharedClip' + animation.duration + animation.easing;
        // Initialize the animation. Set up the clipping rectangle.
        if (init) {
            // If a clipping rectangle with the same properties is currently present in the chart, use that.
            clipRect = chart[sharedClipKey];
            markerClipRect = chart[sharedClipKey + 'm'];
            if (!clipRect) {
                chart[sharedClipKey] = clipRect = renderer.clipRect(
                    extend(clipBox, { width: 0 })
                );
                chart[sharedClipKey + 'm'] = markerClipRect = renderer.clipRect(
                    -99, // include the width of the first marker
                    -UI_CHART_PLOT_TOP,
                    99,
                    chart.chartHeight
                );
            }
            series.group.clip(clipRect);
            series.markerGroup.clip(markerClipRect);
            series.sharedClipKey = sharedClipKey;
        // Run the animation
        } else {
            clipRect = chart[sharedClipKey];
            if (clipRect) {
                clipRect.animate({
                    width: chart.plotSizeX
                }, animation);
                chart[sharedClipKey + 'm'].animate({
                    width: chart.plotSizeX + 99
                }, animation);
            }
            // Delete this function to allow it only once
            series.animate = null;
            // Call the afterAnimate function on animation complete (but don't overwrite the animation.complete option
            // which should be available to the user).
            series.animationTimeout = setTimeout(function () {
                series.afterAnimate();
            }, animation.duration);
        }
    },
    /**
     * This runs after animation to land on the final plot clipping
     */
    afterAnimate: function () {
        let chart = this.chart,
            sharedClipKey = this.sharedClipKey,
            group = this.group,
            trackerGroup = this.trackerGroup;
        if (group && this.options.clip !== false) {
            group.clip(chart.clipRect);
            if (trackerGroup) {
                trackerGroup.clip(chart.clipRect); // #484
            }
            this.markerGroup.clip(); // no clip
        }
        // Remove the shared clipping rectancgle when all series are shown
        setTimeout(function () {
            if (sharedClipKey && chart[sharedClipKey]) {
                chart[sharedClipKey] = chart[sharedClipKey].destroy();
                chart[sharedClipKey + 'm'] = chart[sharedClipKey + 'm'].destroy();
            }
        }, 100);
    },
    /**
     * Draw the markers
     */
    drawPoints: function () {
        let series = this,
            pointAttr,
            points = series.points,
            chart = series.chart,
            plotX,
            plotY,
            i,
            point,
            radius,
            symbol,
            isImage,
            graphic,
            options = series.options,
            seriesMarkerOptions = options.marker,
            pointMarkerOptions,
            enabled,
            isInside,
            markerGroup = series.markerGroup;
        if (seriesMarkerOptions.enabled || series._hasPointMarkers) {
            i = points.length;
            while (i--) {
                point = points[i];
                plotX = point.plotX;
                plotY = point.plotY;
                graphic = point.graphic;
                pointMarkerOptions = point.marker || {};
                enabled = (seriesMarkerOptions.enabled && pointMarkerOptions.enabled === UNDEFINED) || pointMarkerOptions.enabled;
                isInside = chart.isInsidePlot(plotX, plotY);
                // only draw the point if y is defined
                if (enabled && plotY !== UNDEFINED && !isNaN(plotY)) {
                    // shortcuts
                    pointAttr = point.pointAttr[point.selected ? SELECT_STATE : NORMAL_STATE];
                    radius = pointAttr.r;
                    symbol = pick(pointMarkerOptions.symbol, series.symbol);
                    isImage = symbol.indexOf('url') === 0;
                    if (graphic) { // update
                        graphic
                            .attr({ // Since the marker group isn't clipped, each individual marker must be toggled
                                visibility: isInside ? (hasSVG ? 'inherit' : VISIBLE) : HIDDEN
                            })
                            .animate(extend({
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
                        .attr(pointAttr)
                        .add(markerGroup);
                    }
                } else if (graphic) {
                    point.graphic = graphic.destroy(); // #1269
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
        let conversion = this.pointAttrToOptions,
            attr,
            option,
            obj = {};
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
        let series = this,
            normalOptions = defaultPlotOptions[series.type].marker ? series.options.marker : series.options,
            stateOptions = normalOptions.states,
            stateOptionsHover = stateOptions[HOVER_STATE],
            pointStateOptionsHover,
            seriesColor = series.color,
            normalDefaults = {
                stroke: seriesColor,
                fill: seriesColor
            },
            points = series.points || [], // #927
            i,
            point,
            seriesPointAttr = [],
            pointAttr,
            pointAttrToOptions = series.pointAttrToOptions,
            key;
        // series type specific modifications
        // if no hover radius is given, default to normal radius + 2
        stateOptionsHover.radius = stateOptionsHover.radius || normalOptions.radius + 2;
        stateOptionsHover.lineWidth = stateOptionsHover.lineWidth || normalOptions.lineWidth + 1;
        // general point attributes for the series normal state
        seriesPointAttr[NORMAL_STATE] = series.convertAttribs(normalOptions, normalDefaults);
        // HOVER_STATE and SELECT_STATE states inherit from normal state except the default radius
        each([HOVER_STATE, SELECT_STATE], function (state) {
            seriesPointAttr[state] =
                    series.convertAttribs(stateOptions[state], seriesPointAttr[NORMAL_STATE]);
        });
        // set it
        series.pointAttr = seriesPointAttr;
        // Generate the point-specific attribute collections if specific point
        // options are given. If not, create a referance to the series wide point
        // attributes
        i = points.length;
        while (i--) {
            point = points[i];
            normalOptions = (point.options && point.options.marker) || point.options;
            if (normalOptions && normalOptions.enabled === false) {
                normalOptions.radius = 0;
            }
            // no marker config object is created: copy a reference to the series-wide
            // attribute collection
            pointAttr = seriesPointAttr;
            point.pointAttr = pointAttr;
        }
    },
    /**
     * Clear DOM objects and free up memory
     */
    destroy: function () {
        let series = this,
            chart = series.chart,
            issue134 = /AppleWebKit\/533/.test(userAgent),
            destroy,
            i,
            data = series.data || [],
            point,
            prop,
            axis;
        // add event hook
        fireEvent(series, 'destroy');
        // remove all events
        removeEvent(series);
        // erase from axes
        each(['xAxis', 'yAxis'], function (AXIS) {
            axis = series[AXIS];
            if (axis) {
                erase(axis.seriesList, series);
                axis.isDirty = true;
            }
        });
        // destroy all points with their elements
        i = data.length;
        while (i--) {
            point = data[i];
            if (point && point.destroy) {
                point.destroy();
            }
        }
        series.points = null;
        // Clear the animation timeout if we are destroying the series during initial animation
        clearTimeout(series.animationTimeout);
        // destroy all SVGElements associated to the series
        each(['area', 'graph', 'dataLabelsGroup', 'group', 'markerGroup', 'tracker', 'trackerGroup'], function (prop) {
            if (series[prop]) {
                // issue 134 workaround
                destroy = issue134 && prop === 'group' ?
                    'hide' :
                    'destroy';
                series[prop][destroy]();
            }
        });
        // remove from hoverSeries
        if (chart.hoverSeries === series) {
            chart.hoverSeries = null;
        }
        erase(chart.seriesList, series);
        // clear all members
        for (prop in series) {
            delete series[prop];
        }
    },
    /**
     * Return the graph path of a segment
     */
    getSegmentPath: function (segment) {
        let series = this,
            segmentPath = [],
            step = series.options.step;
        // build the segment line
        each(segment, function (point, i) {
            let plotX = point.plotX,
                plotY = point.plotY,
                lastPoint;
            if (series.getPointSpline) { // generate the spline as defined in the SplineSeries object
                segmentPath.push.apply(segmentPath, series.getPointSpline(segment, point, i));
            } else {
                // moveTo or lineTo
                segmentPath.push(i ? L : M);
                // step line?
                if (step && i) {
                    lastPoint = segment[i - 1];
                    if (step === 'right') {
                        segmentPath.push(
                            lastPoint.plotX,
                            plotY
                        );
                    } else if (step === 'center') {
                        segmentPath.push(
                            (lastPoint.plotX + plotX) / 2,
                            lastPoint.plotY,
                            (lastPoint.plotX + plotX) / 2,
                            plotY
                        );
                    } else {
                        segmentPath.push(
                            plotX,
                            lastPoint.plotY
                        );
                    }
                }
                // normal line to next point
                segmentPath.push(
                    point.plotX,
                    point.plotY
                );
            }
        });
        return segmentPath;
    },
    /**
     * Get the graph path
     */
    getGraphPath: function () {
        let series = this,
            graphPath = [],
            segmentPath,
            singlePoints = []; // used in drawTracker
        // Divide into segments and build graph and area paths
        each(series.segments, function (segment) {
            segmentPath = series.getSegmentPath(segment);
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
        let options = this.options,
            graph = this.graph,
            group = this.group,
            color = options.lineColor || this.color,
            lineWidth = options.lineWidth,
            dashStyle =  options.dashStyle,
            attribs,
            graphPath = this.getGraphPath();
        // draw the graph
        if (graph) {
            stop(graph); // cancel running animations, #459
            graph.animate({ d: graphPath });
        } else {
            if (lineWidth) {
                attribs = {
                    stroke: color,
                    'stroke-width': lineWidth,
                    zIndex: 1 // #1069
                };
                if (dashStyle) {
                    attribs.dashstyle = dashStyle;
                }
                this.graph = this.chart.renderer.path(graphPath)
                    .attr(attribs).add(group).shadow(options.shadow);
            }
        }
    },
    /**
     * General abstraction for creating plot groups like series.group, series.trackerGroup, series.dataLabelsGroup and
     * series.markerGroup. On subsequent calls, the group will only be adjusted to the updated plot size.
     */
    plotGroup: function (prop, name, visibility, zIndex, parent) {
        let group = this[prop],
            chart = this.chart,
            xAxis = this.xAxis,
            yAxis = this.yAxis;
        // Generate it on first call
        if (!group) {
            this[prop] = group = chart.renderer.g(name)
                .attr({
                    visibility: visibility,
                    zIndex: zIndex || 0.1 // IE8 needs this
                })
                .add(parent);
        }
        // Place it on first and subsequent (redraw) calls
        group.translate(
            xAxis ? xAxis.left : chart.plotLeft,
            yAxis ? yAxis.top : UI_CHART_PLOT_TOP
        );
        return group;
    },
    /**
     * Render the graph and markers
     */
    render: function () {
        let series = this,
            chart = series.chart,
            group,
            options = series.options,
            animation = options.animation,
            doAnimation = animation && !!series.animate,
            visibility = series.visible ? VISIBLE : HIDDEN,
            zIndex = options.zIndex,
            hasRendered = series.hasRendered,
            chartSeriesGroup = chart.seriesGroup;
        // the group
        group = series.plotGroup(
            'group',
            'series',
            visibility,
            zIndex,
            chartSeriesGroup
        );
        series.markerGroup = series.plotGroup(
            'markerGroup',
            'markers',
            visibility,
            zIndex,
            chartSeriesGroup
        );
        // initiate the animation
        if (doAnimation) {
            series.animate(true);
        }
        // cache attributes for shapes
        series.getAttribs();
        // draw the graph if any
        if (series.drawGraph) {
            series.drawGraph();
        }
        // draw the points
        series.drawPoints();
        // draw the mouse tracking area
        if (series.options.enableMouseTracking !== false) {
            series.drawTracker();
        }
        // Initial clipping, must be defined after inverting groups for VML
        if (options.clip !== false && !series.sharedClipKey && !hasRendered) {
            group.clip(chart.clipRect);
            if (this.trackerGroup) {
                this.trackerGroup.clip(chart.clipRect);
            }
        }
        // Run the animation
        if (doAnimation) {
            series.animate();
        } else if (!hasRendered) {
            series.afterAnimate();
        }
        series.isDirty = series.isDirtyData = false; // means data is in accordance with what you see
        // (See #322) series.isDirty = series.isDirtyData = false; // means data is in accordance with what you see
        series.hasRendered = true;
    },
    /**
     * Redraw the series after an update in the axes.
     */
    redraw: function () {
        let series = this,
            chart = series.chart,
            wasDirtyData = series.isDirtyData, // cache it here as it is set to false in render, but used after
            group = series.group;
        // reposition on resize
        if (group) {
            group.animate({
                translateX: series.xAxis.left,
                translateY: series.yAxis.top
            });
        }
        series.translate();
        series.setTooltipPoints(true);
        series.render();
        if (wasDirtyData) {
            fireEvent(series, 'updatedData');
        }
    },
    /**
     * Set the state of the graph
     */
    setState: function (state) {
        let series = this,
            options = series.options,
            graph = series.graph,
            stateOptions = options.states,
            lineWidth = options.lineWidth;
        state = state || NORMAL_STATE;
        if (series.state !== state) {
            series.state = state;
            if (stateOptions[state] && stateOptions[state].enabled === false) {
                return;
            }
            if (state) {
                lineWidth = stateOptions[state].lineWidth || lineWidth + 1;
            }
            if (graph && !graph.dashstyle) { // hover is turned off for dashed lines in VML
                graph.attr({ // use attr because animate will cause any other animation on the graph to stop
                    'stroke-width': lineWidth
                }, state ? 0 : 500);
            }
        }
    },
    /**
     * Set the visibility of the graph
     *
     * @param vis {Boolean} True to show the series, false to hide. If UNDEFINED,
     *        the visibility is toggled.
     */
    setVisible: function (vis, redraw) {
        let series = this,
            chart = series.chart,
            seriesGroup = series.group,
            seriesTracker = series.tracker,
            dataLabelsGroup = series.dataLabelsGroup,
            markerGroup = series.markerGroup,
            showOrHide,
            i,
            points = series.points,
            point,
            ignoreHiddenSeries = chart.options.chart.ignoreHiddenSeries,
            oldVisibility = series.visible;
        // if called without an argument, toggle visibility
        series.visible = vis = vis === UNDEFINED ? !oldVisibility : vis;
        showOrHide = vis ? 'show' : 'hide';
        // show or hide series
        if (seriesGroup) { // pies don't have one
            seriesGroup[showOrHide]();
        }
        if (markerGroup) {
            markerGroup[showOrHide]();
        }
        // show or hide trackers
        if (seriesTracker) {
            seriesTracker[showOrHide]();
        } else if (points) {
            i = points.length;
            while (i--) {
                point = points[i];
                if (point.tracker) {
                    point.tracker[showOrHide]();
                }
            }
        }
        // hide tooltip (#1361)
        if (chart.hoverSeries === series) {
            series.onMouseOut();
        }
        if (dataLabelsGroup) {
            dataLabelsGroup[showOrHide]();
        }
        // rescale or adapt to resized chart
        series.isDirty = true;
        if (ignoreHiddenSeries) {
            chart.isDirtyBox = true;
        }
        if (redraw !== false) {
            chart.redraw();
        }
        fireEvent(series, showOrHide);
    },
    /**
     * Show the graph
     */
    show: function () {
        this.setVisible(true);
    },
    /**
     * Hide the graph
     */
    hide: function () {
        this.setVisible(false);
    },
    /**
     * Set the selected state of the graph
     *
     * @param selected {Boolean} True to select the series, false to unselect. If
     *        UNDEFINED, the selection state is toggled.
     */
    select: function (selected) {
        let series = this;
        // if called without an argument, toggle
        series.selected = selected = (selected === UNDEFINED) ? !series.selected : selected;
        if (series.checkbox) {
            series.checkbox.checked = selected;
        }
        fireEvent(series, selected ? 'select' : 'unselect');
    },
    /**
     * Draw the tracker object that sits above all data labels and markers to
     * track mouse events on the graph or points. For the line type charts
     * the tracker uses the same graphPath, but with a greater stroke width
     * for better control.
     */
    drawTracker: function () {
        let series = this,
            options = series.options,
            trackByArea = options.trackByArea,
            trackerPath = [].concat(trackByArea ? series.areaPath : series.graphPath),
            trackerPathLength = trackerPath.length,
            chart = series.chart,
            renderer = chart.renderer,
            snap = chart.options.tooltip.snap,
            tracker = series.tracker,
            cursor = options.cursor,
            css = cursor && { cursor: cursor },
            singlePoints = series.singlePoints,
            trackerGroup = this.isCartesian && this.plotGroup('trackerGroup', null, VISIBLE, options.zIndex || 1, chart.trackerGroup),
            singlePoint,
            i,
            onMouseOver = function () {
                if (chart.hoverSeries !== series) {
                    series.onMouseOver();
                }
            },
            onMouseOut = function () {
                if (!options.stickyTracking) {
                    series.onMouseOut();
                }
            };
        // Extend end points. A better way would be to use round linecaps,
        // but those are not clickable in VML.
        if (trackerPathLength && !trackByArea) {
            i = trackerPathLength + 1;
            while (i--) {
                if (trackerPath[i] === M) { // extend left side
                    trackerPath.splice(i + 1, 0, trackerPath[i + 1] - snap, trackerPath[i + 2], L);
                }
                if ((i && trackerPath[i] === M) || i === trackerPathLength) { // extend right side
                    trackerPath.splice(i, 0, L, trackerPath[i - 2] + snap, trackerPath[i - 1]);
                }
            }
        }
        // handle single points
        for (i = 0; i < singlePoints.length; i++) {
            singlePoint = singlePoints[i];
            trackerPath.push(M, singlePoint.plotX - snap, singlePoint.plotY,
                L, singlePoint.plotX + snap, singlePoint.plotY);
        }
        // draw the tracker
        if (tracker) {
            tracker.attr({ d: trackerPath });
        } else { // create
            series.tracker = tracker = renderer.path(trackerPath)
                .attr({
                    isTracker: true,
                    'stroke-linejoin': 'round', // #1225
                    visibility: series.visible ? VISIBLE : HIDDEN,
                    stroke: TRACKER_FILL,
                    fill: trackByArea ? TRACKER_FILL : NONE,
                    'stroke-width' : options.lineWidth + (trackByArea ? 0 : 2 * snap)
                })
                .on('mouseover', onMouseOver)
                .on('mouseout', onMouseOut)
                .css(css)
                .add(trackerGroup);
            if (hasTouch) {
                tracker.on('touchstart', onMouseOver);
            }
        }
    }
}; // end Series prototype
/**
 * LineSeries object
 */
let LineSeries = extendClass(Series);
seriesTypes.line = LineSeries;
// global variables
extend(Highcharts, {
    Chart: Chart
});
/*jslint-enable*/
function uichartResize({
    uichart
}) {
    let chartHeight;
    let chartWidth;
    // save old height and width
    uichart.oldChartHeight = uichart.chartHeight;
    uichart.oldChartWidth = uichart.chartWidth;
    // get new height and width
    uichart.container.remove();
    chartHeight = uichart.renderTo.clientHeight;
    chartWidth = uichart.renderTo.clientWidth;
    uichart.renderTo.appendChild(uichart.container);
    uichart.chartWidth = chartWidth;
    uichart.chartHeight = chartHeight;
    // increment / decrement isResizing
    uichart.isResizing += 1;
    setTimeout(function () {
        uichart.isResizing -= 1;
    }, 500);
    uichart.container.style = (
        `height: ${chartHeight}px; width: ${chartWidth}px;`
    );
    uichart.renderer.setSize(chartWidth, chartHeight, true);
    // update axis lengths for more correct tick intervals:
    uichart.plotWidth = chartWidth - uichart.plotLeft - UI_CHART_MARGIN_RIGHT;
    uichart.plotHeight = (
        chartHeight
        - UI_CHART_PLOT_TOP
        - UI_CHART_MARGIN_BOTTOM
    );
    // handle axes
    uichart.maxTicks = null;
    uichart.axisList.forEach(function (axis) {
        axis.isDirty = true;
        axis.setScale();
    });
    // make sure non-cartesian series are also handled
    uichart.seriesList.forEach(function (series) {
        series.isDirty = true;
    });
    uichart.isDirtyBox = true; // force redraw of plot and chart border
    uichart.getMargins();
    uichart.redraw(true);
    uichart.oldChartHeight = null;
}
function uichartXY(uichart, {
    pageX,
    pageY
}) {
// this function will get mouse position chartX, chartY relative to svg-chart
    let rect = uichart.container.getBoundingClientRect();
    return [
        Math.round(
            pageX
            - rect.left
            - window.scrollX
            + document.documentElement.clientLeft
            - uichart.plotLeft
        ),
        Math.round(
            pageY
            - rect.top
            - window.scrollY
            + document.documentElement.clientTop
            - UI_CHART_PLOT_TOP
        )
    ];
}
window.uichartResize = uichartResize;
window.uichartXY = uichartXY;
/*jslint-disable*/
}());
/*jslint-enable*/
