/*jslint-disable*/
// Copyright (c) 2021 Kai Zhu
// SPDX-License-Identifier: MIT
// 2022-11-21T06:02:34+0000
(function () {
"use strict";

var f;f||(f=typeof Module !== 'undefined' ? Module : {});var aa=Object.assign;(function(){function a(){}function b(...c){a("\n\ndebugInline");a(...c);a("\n");return c[0]}b();a=console.error;return b})();
(async function(){function a(k){let l=k.argList,n=new Uint8Array(k.baton&&k.baton.buffer);var p=0;let t=k.cFuncName,u=l[4];var D=p=0;let Y="",Jb=k.id;D=l[2];switch(t){case "_dbClose":case "_dbExec":case "_dbFileImportOrExport":case "_dbNoop":case "_dbOpen":p=ba(n.byteLength);k.batonPtr=p;h.set(n,p);switch(t){case "_dbClose":k=d(p,1);console.error(`_dbClose("${k}")`);break;case "_dbFileImportOrExport":D||ca(new Uint8Array(u));break;case "_dbOpen":k=d(p,0),console.error(`_dbOpen("${k}")`)}f[t](p);Y=
c(p);n.set(new Uint8Array(h.buffer,h.byteOffset+p,1024));n=new BigInt64Array(n.buffer,8);n.subarray(16,32).forEach(function(E,F){l[F]&&(l[F].constructor===ArrayBuffer||"function"===typeof SharedArrayBuffer&&l[F].constructor===SharedArrayBuffer)||(E=Number(E),0===E?l[F]=n[F]:(l[F]=new ArrayBuffer(Number(n[F])),(new Uint8Array(l[F])).set(h.subarray(E,E+l[F].byteLength)),da(E)))});switch(!Y&&t){case "_dbFileImportOrExport":D&&(u=ea(),l[4]=u.buffer);break;case "_dbOpen":u&&(ca(new Uint8Array(u)),p=Number(l[0]),
(D=b(p,"/tmp/__dbtmp1",0))&&(Y=g(p)))}postMessage({argList:l,baton:new DataView(n.buffer),cFuncName:t,errmsg:Y,id:Jb},[n.buffer,...l.filter(function(E){return E&&E.constructor===ArrayBuffer})]);return}throw Error(`invalid cFuncName "${t}"`);}let b,c,d,e,g;globalThis.onmessage=async function({data:k}){let l=k.cFuncName;await e;k.batonPtr=0;try{await a(k)}catch(n){postMessage({errmsg:n.stack,id:k.id})}finally{switch(da(k.batonPtr),l){case "_dbFileImportOrExport":case "_dbOpen":try{fa("/tmp/__dbtmp1")}catch(n){}}}};
e=new Promise(function(k){f.postRun=k});await e;b=m("__dbFileImportOrExport","number",["number","string","number"]);c=m("jsbatonValueErrmsg","string",["number"]);d=m("jsbatonValueStringArgi","string",["number","number"]);g=m("sqlite3_errmsg","string",["number"]);ha()})();var ia=aa({},f),ja="./this.program",ka="object"===typeof window,q="function"===typeof importScripts,r="object"===typeof process&&"object"===typeof process.versions&&"string"===typeof process.versions.node,v="",la,ma,na,fs,oa,pa;
if(r)v=q?require("path").dirname(v)+"/":__dirname+"/",pa=()=>{oa||(fs=require("fs"),oa=require("path"))},la=function(a,b){pa();a=oa.normalize(a);return fs.readFileSync(a,b?null:"utf8")},na=a=>{a=la(a,!0);a.buffer||(a=new Uint8Array(a));return a},ma=(a,b,c)=>{pa();a=oa.normalize(a);fs.readFile(a,function(d,e){d?c(d):b(e.buffer)})},1<process.argv.length&&(ja=process.argv[1].replace(/\\/g,"/")),process.argv.slice(2),"undefined"!==typeof module&&(module.exports=f),f.inspect=function(){return"[Emscripten Module object]"};
else if(ka||q)q?v=self.location.href:"undefined"!==typeof document&&document.currentScript&&(v=document.currentScript.src),v=0!==v.indexOf("blob:")?v.substr(0,v.replace(/[?#].*/,"").lastIndexOf("/")+1):"",la=a=>{var b=new XMLHttpRequest;b.open("GET",a,!1);b.send(null);return b.responseText},q&&(na=a=>{var b=new XMLHttpRequest;b.open("GET",a,!1);b.responseType="arraybuffer";b.send(null);return new Uint8Array(b.response)}),ma=(a,b,c)=>{var d=new XMLHttpRequest;d.open("GET",a,!0);d.responseType="arraybuffer";
d.onload=()=>{200==d.status||0==d.status&&d.response?b(d.response):c()};d.onerror=c;d.send(null)};var qa=f.print||console.log.bind(console),w=f.printErr||console.warn.bind(console);aa(f,ia);ia=null;f.thisProgram&&(ja=f.thisProgram);var x;f.wasmBinary&&(x=f.wasmBinary);var noExitRuntime=f.noExitRuntime||!0;"object"!==typeof WebAssembly&&y("no native wasm support detected");var ra,sa=!1;
function ta(a,b,c,d){var e={string:function(p){var t=0;if(null!==p&&void 0!==p&&0!==p){var u=(p.length<<2)+1;t=ua(u);z(p,h,t,u)}return t},array:function(p){var t=ua(p.length);A.set(p,t);return t}};a=f["_"+a];var g=[],k=0;if(d)for(var l=0;l<d.length;l++){var n=e[c[l]];n?(0===k&&(k=va()),g[l]=n(d[l])):g[l]=d[l]}c=a.apply(null,g);return c=function(p){0!==k&&wa(k);return"string"===b?B(p):"boolean"===b?!!p:p}(c)}
function m(a,b,c,d){c=c||[];var e=c.every(function(g){return"number"===g});return"string"!==b&&e&&!d?f["_"+a]:function(){return ta(a,b,c,arguments)}}var xa="undefined"!==typeof TextDecoder?new TextDecoder("utf8"):void 0;
function C(a,b,c){var d=b+c;for(c=b;a[c]&&!(c>=d);)++c;if(16<c-b&&a.subarray&&xa)return xa.decode(a.subarray(b,c));for(d="";b<c;){var e=a[b++];if(e&128){var g=a[b++]&63;if(192==(e&224))d+=String.fromCharCode((e&31)<<6|g);else{var k=a[b++]&63;e=224==(e&240)?(e&15)<<12|g<<6|k:(e&7)<<18|g<<12|k<<6|a[b++]&63;65536>e?d+=String.fromCharCode(e):(e-=65536,d+=String.fromCharCode(55296|e>>10,56320|e&1023))}}else d+=String.fromCharCode(e)}return d}function B(a){return a?C(h,a,void 0):""}
function z(a,b,c,d){if(!(0<d))return 0;var e=c;d=c+d-1;for(var g=0;g<a.length;++g){var k=a.charCodeAt(g);if(55296<=k&&57343>=k){var l=a.charCodeAt(++g);k=65536+((k&1023)<<10)|l&1023}if(127>=k){if(c>=d)break;b[c++]=k}else{if(2047>=k){if(c+1>=d)break;b[c++]=192|k>>6}else{if(65535>=k){if(c+2>=d)break;b[c++]=224|k>>12}else{if(c+3>=d)break;b[c++]=240|k>>18;b[c++]=128|k>>12&63}b[c++]=128|k>>6&63}b[c++]=128|k&63}}b[c]=0;return c-e}
function G(a){for(var b=0,c=0;c<a.length;++c){var d=a.charCodeAt(c);55296<=d&&57343>=d&&(d=65536+((d&1023)<<10)|a.charCodeAt(++c)&1023);127>=d?++b:b=2047>=d?b+2:65535>=d?b+3:b+4}return b}function ya(a){var b=G(a)+1,c=za(b);c&&z(a,A,c,b);return c}var Aa,A,h,Ba,H,Ca;
function Da(){var a=ra.buffer;Aa=a;f.HEAP8=A=new Int8Array(a);f.HEAP16=Ba=new Int16Array(a);f.HEAP32=H=new Int32Array(a);f.HEAPU8=h=new Uint8Array(a);f.HEAPU16=new Uint16Array(a);f.HEAPU32=new Uint32Array(a);f.HEAPF32=new Float32Array(a);f.HEAPF64=new Float64Array(a);f.HEAP64=Ca=new BigInt64Array(a);f.HEAPU64=new BigUint64Array(a)}var Ea,Fa=[],Ga=[],Ha=[];function Ia(){var a=f.preRun.shift();Fa.unshift(a)}var I=0,Ja=null,J=null;f.preloadedImages={};f.preloadedAudios={};
function y(a){if(f.onAbort)f.onAbort(a);a="Aborted("+a+")";w(a);sa=!0;throw new WebAssembly.RuntimeError(a+". Build with -s ASSERTIONS=1 for more info.");}function Ka(){return K.startsWith("data:application/octet-stream;base64,")}var K;K="sqlmath_wasm.wasm";if(!Ka()){var La=K;K=f.locateFile?f.locateFile(La,v):v+La}function Ma(){var a=K;try{if(a==K&&x)return new Uint8Array(x);if(na)return na(a);throw"both async and sync fetching of the wasm failed";}catch(b){y(b)}}
function Na(){if(!x&&(ka||q)){if("function"===typeof fetch&&!K.startsWith("file://"))return fetch(K,{credentials:"same-origin"}).then(function(a){if(!a.ok)throw"failed to load wasm binary file at '"+K+"'";return a.arrayBuffer()}).catch(function(){return Ma()});if(ma)return new Promise(function(a,b){ma(K,function(c){a(new Uint8Array(c))},b)})}return Promise.resolve().then(function(){return Ma()})}
function Oa(a){for(;0<a.length;){var b=a.shift();if("function"==typeof b)b(f);else{var c=b.Va;"number"===typeof c?void 0===b.Aa?Ea.get(c)():Ea.get(c)(b.Aa):c(void 0===b.Aa?null:b.Aa)}}}function Pa(a){return a.replace(/\b_Z[\w\d_]+/g,function(b){return b===b?b:b+" ["+b+"]"})}function Qa(a,b){for(var c=0,d=a.length-1;0<=d;d--){var e=a[d];"."===e?a.splice(d,1):".."===e?(a.splice(d,1),c++):c&&(a.splice(d,1),c--)}if(b)for(;c;c--)a.unshift("..");return a}
function L(a){var b="/"===a.charAt(0),c="/"===a.substr(-1);(a=Qa(a.split("/").filter(function(d){return!!d}),!b).join("/"))||b||(a=".");a&&c&&(a+="/");return(b?"/":"")+a}function Ra(a){var b=/^(\/?|)([\s\S]*?)((?:\.{1,2}|[^\/]+?|)(\.[^.\/]*|))(?:[\/]*)$/.exec(a).slice(1);a=b[0];b=b[1];if(!a&&!b)return".";b&&(b=b.substr(0,b.length-1));return a+b}function Sa(a){if("/"===a)return"/";a=L(a);a=a.replace(/\/$/,"");var b=a.lastIndexOf("/");return-1===b?a:a.substr(b+1)}
function Ta(){if("object"===typeof crypto&&"function"===typeof crypto.getRandomValues){var a=new Uint8Array(1);return function(){crypto.getRandomValues(a);return a[0]}}if(r)try{var b=require("crypto");return function(){return b.randomBytes(1)[0]}}catch(c){}return function(){y("randomDevice")}}
function Ua(){for(var a="",b=!1,c=arguments.length-1;-1<=c&&!b;c--){b=0<=c?arguments[c]:"/";if("string"!==typeof b)throw new TypeError("Arguments to path.resolve must be strings");if(!b)return"";a=b+"/"+a;b="/"===b.charAt(0)}a=Qa(a.split("/").filter(function(d){return!!d}),!b).join("/");return(b?"/":"")+a||"."}var Va=[];function Wa(a,b){Va[a]={input:[],output:[],va:b};Xa(a,Ya)}
var Ya={open:function(a){var b=Va[a.node.rdev];if(!b)throw new M(43);a.tty=b;a.seekable=!1},close:function(a){a.tty.va.flush(a.tty)},flush:function(a){a.tty.va.flush(a.tty)},read:function(a,b,c,d){if(!a.tty||!a.tty.va.Ia)throw new M(60);for(var e=0,g=0;g<d;g++){try{var k=a.tty.va.Ia(a.tty)}catch(l){throw new M(29);}if(void 0===k&&0===e)throw new M(6);if(null===k||void 0===k)break;e++;b[c+g]=k}e&&(a.node.timestamp=Date.now());return e},write:function(a,b,c,d){if(!a.tty||!a.tty.va.Ca)throw new M(60);
try{for(var e=0;e<d;e++)a.tty.va.Ca(a.tty,b[c+e])}catch(g){throw new M(29);}d&&(a.node.timestamp=Date.now());return e}},Za={Ia:function(a){if(!a.input.length){var b=null;if(r){var c=Buffer.alloc(256),d=0;try{d=fs.readSync(process.stdin.fd,c,0,256,null)}catch(e){if(e.toString().includes("EOF"))d=0;else throw e;}0<d?b=c.slice(0,d).toString("utf-8"):b=null}else"undefined"!=typeof window&&"function"==typeof window.prompt?(b=window.prompt("Input: "),null!==b&&(b+="\n")):"function"==typeof readline&&(b=
readline(),null!==b&&(b+="\n"));if(!b)return null;c=Array(G(b)+1);b=z(b,c,0,c.length);c.length=b;a.input=c}return a.input.shift()},Ca:function(a,b){null===b||10===b?(qa(C(a.output,0)),a.output=[]):0!=b&&a.output.push(b)},flush:function(a){a.output&&0<a.output.length&&(qa(C(a.output,0)),a.output=[])}},$a={Ca:function(a,b){null===b||10===b?(w(C(a.output,0)),a.output=[]):0!=b&&a.output.push(b)},flush:function(a){a.output&&0<a.output.length&&(w(C(a.output,0)),a.output=[])}};
function ab(a){a=65536*Math.ceil(a/65536);var b=bb(65536,a);if(!b)return 0;h.fill(0,b,b+a);return b}
var N={pa:null,qa:function(){return N.createNode(null,"/",16895,0)},createNode:function(a,b,c,d){if(24576===(c&61440)||4096===(c&61440))throw new M(63);N.pa||(N.pa={dir:{node:{oa:N.ia.oa,na:N.ia.na,lookup:N.ia.lookup,wa:N.ia.wa,rename:N.ia.rename,unlink:N.ia.unlink,rmdir:N.ia.rmdir,readdir:N.ia.readdir,symlink:N.ia.symlink},stream:{sa:N.ja.sa}},file:{node:{oa:N.ia.oa,na:N.ia.na},stream:{sa:N.ja.sa,read:N.ja.read,write:N.ja.write,Ea:N.ja.Ea,xa:N.ja.xa,ya:N.ja.ya}},link:{node:{oa:N.ia.oa,na:N.ia.na,
readlink:N.ia.readlink},stream:{}},Fa:{node:{oa:N.ia.oa,na:N.ia.na},stream:cb}});c=db(a,b,c,d);16384===(c.mode&61440)?(c.ia=N.pa.dir.node,c.ja=N.pa.dir.stream,c.ka={}):32768===(c.mode&61440)?(c.ia=N.pa.file.node,c.ja=N.pa.file.stream,c.ma=0,c.ka=null):40960===(c.mode&61440)?(c.ia=N.pa.link.node,c.ja=N.pa.link.stream):8192===(c.mode&61440)&&(c.ia=N.pa.Fa.node,c.ja=N.pa.Fa.stream);c.timestamp=Date.now();a&&(a.ka[b]=c,a.timestamp=c.timestamp);return c},Wa:function(a){return a.ka?a.ka.subarray?a.ka.subarray(0,
a.ma):new Uint8Array(a.ka):new Uint8Array(0)},Ga:function(a,b){var c=a.ka?a.ka.length:0;c>=b||(b=Math.max(b,c*(1048576>c?2:1.125)>>>0),0!=c&&(b=Math.max(b,256)),c=a.ka,a.ka=new Uint8Array(b),0<a.ma&&a.ka.set(c.subarray(0,a.ma),0))},Sa:function(a,b){if(a.ma!=b)if(0==b)a.ka=null,a.ma=0;else{var c=a.ka;a.ka=new Uint8Array(b);c&&a.ka.set(c.subarray(0,Math.min(b,a.ma)));a.ma=b}},ia:{oa:function(a){var b={};b.dev=8192===(a.mode&61440)?a.id:1;b.ino=a.id;b.mode=a.mode;b.nlink=1;b.uid=0;b.gid=0;b.rdev=a.rdev;
16384===(a.mode&61440)?b.size=4096:32768===(a.mode&61440)?b.size=a.ma:40960===(a.mode&61440)?b.size=a.link.length:b.size=0;b.atime=new Date(a.timestamp);b.mtime=new Date(a.timestamp);b.ctime=new Date(a.timestamp);b.Ka=4096;b.blocks=Math.ceil(b.size/b.Ka);return b},na:function(a,b){void 0!==b.mode&&(a.mode=b.mode);void 0!==b.timestamp&&(a.timestamp=b.timestamp);void 0!==b.size&&N.Sa(a,b.size)},lookup:function(){throw eb[44];},wa:function(a,b,c,d){return N.createNode(a,b,c,d)},rename:function(a,b,c){if(16384===
(a.mode&61440)){try{var d=O(b,c)}catch(g){}if(d)for(var e in d.ka)throw new M(55);}delete a.parent.ka[a.name];a.parent.timestamp=Date.now();a.name=c;b.ka[c]=a;b.timestamp=a.parent.timestamp;a.parent=b},unlink:function(a,b){delete a.ka[b];a.timestamp=Date.now()},rmdir:function(a,b){var c=O(a,b),d;for(d in c.ka)throw new M(55);delete a.ka[b];a.timestamp=Date.now()},readdir:function(a){var b=[".",".."],c;for(c in a.ka)a.ka.hasOwnProperty(c)&&b.push(c);return b},symlink:function(a,b,c){a=N.createNode(a,
b,41471,0);a.link=c;return a},readlink:function(a){if(40960!==(a.mode&61440))throw new M(28);return a.link}},ja:{read:function(a,b,c,d,e){var g=a.node.ka;if(e>=a.node.ma)return 0;a=Math.min(a.node.ma-e,d);if(8<a&&g.subarray)b.set(g.subarray(e,e+a),c);else for(d=0;d<a;d++)b[c+d]=g[e+d];return a},write:function(a,b,c,d,e,g){b.buffer===A.buffer&&(g=!1);if(!d)return 0;a=a.node;a.timestamp=Date.now();if(b.subarray&&(!a.ka||a.ka.subarray)){if(g)return a.ka=b.subarray(c,c+d),a.ma=d;if(0===a.ma&&0===e)return a.ka=
b.slice(c,c+d),a.ma=d;if(e+d<=a.ma)return a.ka.set(b.subarray(c,c+d),e),d}N.Ga(a,e+d);if(a.ka.subarray&&b.subarray)a.ka.set(b.subarray(c,c+d),e);else for(g=0;g<d;g++)a.ka[e+g]=b[c+g];a.ma=Math.max(a.ma,e+d);return d},sa:function(a,b,c){1===c?b+=a.position:2===c&&32768===(a.node.mode&61440)&&(b+=a.node.ma);if(0>b)throw new M(28);return b},Ea:function(a,b,c){N.Ga(a.node,b+c);a.node.ma=Math.max(a.node.ma,b+c)},xa:function(a,b,c,d,e,g){if(0!==b)throw new M(28);if(32768!==(a.node.mode&61440))throw new M(43);
a=a.node.ka;if(g&2||a.buffer!==Aa){if(0<d||d+c<a.length)a.subarray?a=a.subarray(d,d+c):a=Array.prototype.slice.call(a,d,d+c);d=!0;c=ab(c);if(!c)throw new M(48);A.set(a,c)}else d=!1,c=a.byteOffset;return{Ra:c,za:d}},ya:function(a,b,c,d,e){if(32768!==(a.node.mode&61440))throw new M(43);if(e&2)return 0;N.ja.write(a,b,0,d,c,!1);return 0}}},fb=null,gb={},P=[],hb=1,Q=null,ib=!0,M=null,eb={},R=(a,b={})=>{a=Ua("/",a);if(!a)return{path:"",node:null};var c={Ha:!0,Da:0},d;for(d in c)void 0===b[d]&&(b[d]=c[d]);
if(8<b.Da)throw new M(32);a=Qa(a.split("/").filter(k=>!!k),!1);var e=fb;c="/";for(d=0;d<a.length;d++){var g=d===a.length-1;if(g&&b.parent)break;e=O(e,a[d]);c=L(c+"/"+a[d]);e.ta&&(!g||g&&b.Ha)&&(e=e.ta.root);if(!g||b.ra)for(g=0;40960===(e.mode&61440);)if(e=jb(c),c=Ua(Ra(c),e),e=R(c,{Da:b.Da}).node,40<g++)throw new M(32);}return{path:c,node:e}},kb=a=>{for(var b;;){if(a===a.parent)return a=a.qa.Ja,b?"/"!==a[a.length-1]?a+"/"+b:a+b:a;b=b?a.name+"/"+b:a.name;a=a.parent}},lb=(a,b)=>{for(var c=0,d=0;d<b.length;d++)c=
(c<<5)-c+b.charCodeAt(d)|0;return(a+c>>>0)%Q.length},mb=a=>{var b=lb(a.parent.id,a.name);if(Q[b]===a)Q[b]=a.ua;else for(b=Q[b];b;){if(b.ua===a){b.ua=a.ua;break}b=b.ua}},O=(a,b)=>{var c;if(c=(c=S(a,"x"))?c:a.ia.lookup?0:2)throw new M(c,a);for(c=Q[lb(a.id,b)];c;c=c.ua){var d=c.name;if(c.parent.id===a.id&&d===b)return c}return a.ia.lookup(a,b)},db=(a,b,c,d)=>{a=new nb(a,b,c,d);b=lb(a.parent.id,a.name);a.ua=Q[b];return Q[b]=a},ob={r:0,"r+":2,w:577,"w+":578,a:1089,"a+":1090},pb=a=>{var b=["r","w","rw"][a&
3];a&512&&(b+="w");return b},S=(a,b)=>{if(ib)return 0;if(!b.includes("r")||a.mode&292){if(b.includes("w")&&!(a.mode&146)||b.includes("x")&&!(a.mode&73))return 2}else return 2;return 0},qb=(a,b)=>{try{return O(a,b),20}catch(c){}return S(a,"wx")},rb=(a,b,c)=>{try{var d=O(a,b)}catch(e){return e.la}if(a=S(a,"wx"))return a;if(c){if(16384!==(d.mode&61440))return 54;if(d===d.parent||"/"===kb(d))return 10}else if(16384===(d.mode&61440))return 31;return 0},sb=(a=0,b=4096)=>{for(;a<=b;a++)if(!P[a])return a;
throw new M(33);},ub=(a,b)=>{tb||(tb=function(){},tb.prototype={});var c=new tb,d;for(d in a)c[d]=a[d];a=c;b=sb(b,void 0);a.fd=b;return P[b]=a},cb={open:a=>{a.ja=gb[a.node.rdev].ja;a.ja.open&&a.ja.open(a)},sa:()=>{throw new M(70);}},Xa=(a,b)=>{gb[a]={ja:b}},vb=(a,b)=>{var c="/"===b,d=!b;if(c&&fb)throw new M(10);if(!c&&!d){var e=R(b,{Ha:!1});b=e.path;e=e.node;if(e.ta)throw new M(10);if(16384!==(e.mode&61440))throw new M(54);}b={type:a,Xa:{},Ja:b,Pa:[]};a=a.qa(b);a.qa=b;b.root=a;c?fb=a:e&&(e.ta=b,e.qa&&
e.qa.Pa.push(b))},wb=(a,b,c)=>{var d=R(a,{parent:!0}).node;a=Sa(a);if(!a||"."===a||".."===a)throw new M(28);var e=qb(d,a);if(e)throw new M(e);if(!d.ia.wa)throw new M(63);return d.ia.wa(d,a,b,c)},T=(a,b)=>wb(a,(void 0!==b?b:511)&1023|16384,0),xb=(a,b,c)=>{"undefined"===typeof c&&(c=b,b=438);wb(a,b|8192,c)},yb=(a,b)=>{if(!Ua(a))throw new M(44);var c=R(b,{parent:!0}).node;if(!c)throw new M(44);b=Sa(b);var d=qb(c,b);if(d)throw new M(d);if(!c.ia.symlink)throw new M(63);c.ia.symlink(c,b,a)},fa=a=>{var b=
R(a,{parent:!0}).node;if(!b)throw new M(44);a=Sa(a);var c=O(b,a),d=rb(b,a,!1);if(d)throw new M(d);if(!b.ia.unlink)throw new M(63);if(c.ta)throw new M(10);b.ia.unlink(b,a);mb(c)},jb=a=>{a=R(a).node;if(!a)throw new M(44);if(!a.ia.readlink)throw new M(28);return Ua(kb(a.parent),a.ia.readlink(a))},U=(a,b)=>{a=R(a,{ra:!b}).node;if(!a)throw new M(44);if(!a.ia.oa)throw new M(63);return a.ia.oa(a)},zb=a=>U(a,!0),Ab=(a,b)=>{a="string"===typeof a?R(a,{ra:!0}).node:a;if(!a.ia.na)throw new M(63);a.ia.na(a,{mode:b&
4095|a.mode&-4096,timestamp:Date.now()})},Bb=a=>{a="string"===typeof a?R(a,{ra:!0}).node:a;if(!a.ia.na)throw new M(63);a.ia.na(a,{timestamp:Date.now()})},Cb=(a,b)=>{if(0>b)throw new M(28);a="string"===typeof a?R(a,{ra:!0}).node:a;if(!a.ia.na)throw new M(63);if(16384===(a.mode&61440))throw new M(31);if(32768!==(a.mode&61440))throw new M(28);var c=S(a,"w");if(c)throw new M(c);a.ia.na(a,{size:b,timestamp:Date.now()})},V=(a,b,c,d)=>{if(""===a)throw new M(44);if("string"===typeof b){var e=ob[b];if("undefined"===
typeof e)throw Error("Unknown file open mode: "+b);b=e}c=b&64?("undefined"===typeof c?438:c)&4095|32768:0;if("object"===typeof a)var g=a;else{a=L(a);try{g=R(a,{ra:!(b&131072)}).node}catch(k){}}e=!1;if(b&64)if(g){if(b&128)throw new M(20);}else g=wb(a,c,0),e=!0;if(!g)throw new M(44);8192===(g.mode&61440)&&(b&=-513);if(b&65536&&16384!==(g.mode&61440))throw new M(54);if(!e&&(c=g?40960===(g.mode&61440)?32:16384===(g.mode&61440)&&("r"!==pb(b)||b&512)?31:S(g,pb(b)):44))throw new M(c);b&512&&Cb(g,0);b&=-131713;
d=ub({node:g,path:kb(g),flags:b,seekable:!0,position:0,ja:g.ja,Ua:[],error:!1},d);d.ja.open&&d.ja.open(d);!f.logReadFiles||b&1||(Db||(Db={}),a in Db||(Db[a]=1));return d},Eb=a=>{if(null===a.fd)throw new M(8);a.Ba&&(a.Ba=null);try{a.ja.close&&a.ja.close(a)}catch(b){throw b;}finally{P[a.fd]=null}a.fd=null},Fb=(a,b,c)=>{if(null===a.fd)throw new M(8);if(!a.seekable||!a.ja.sa)throw new M(70);if(0!=c&&1!=c&&2!=c)throw new M(28);a.position=a.ja.sa(a,b,c);a.Ua=[]},Gb=(a,b,c,d,e)=>{if(0>d||0>e)throw new M(28);
if(null===a.fd)throw new M(8);if(1===(a.flags&2097155))throw new M(8);if(16384===(a.node.mode&61440))throw new M(31);if(!a.ja.read)throw new M(28);var g="undefined"!==typeof e;if(!g)e=a.position;else if(!a.seekable)throw new M(70);b=a.ja.read(a,b,c,d,e);g||(a.position+=b);return b},Hb=(a,b,c,d,e)=>{var g=void 0;if(0>d||0>g)throw new M(28);if(null===a.fd)throw new M(8);if(0===(a.flags&2097155))throw new M(8);if(16384===(a.node.mode&61440))throw new M(31);if(!a.ja.write)throw new M(28);a.seekable&&
a.flags&1024&&Fb(a,0,2);var k="undefined"!==typeof g;if(!k)g=a.position;else if(!a.seekable)throw new M(70);b=a.ja.write(a,b,c,d,g,e);k||(a.position+=b);return b},ea=()=>{var a=a||"binary";if("utf8"!==a&&"binary"!==a)throw Error('Invalid encoding type "'+a+'"');var b;var c=V("/tmp/__dbtmp1",c||0);var d=U("/tmp/__dbtmp1").size,e=new Uint8Array(d);Gb(c,e,0,d,0);"utf8"===a?b=C(e,0):"binary"===a&&(b=e);Eb(c);return b},ca=a=>{var b={};b.flags=b.flags||577;var c=V("/tmp/__dbtmp1",b.flags,b.mode);if("string"===
typeof a){var d=new Uint8Array(G(a)+1);a=z(a,d,0,d.length);Hb(c,d,0,a,b.Ma)}else if(ArrayBuffer.isView(a))Hb(c,a,0,a.byteLength,b.Ma);else throw Error("Unsupported data type");Eb(c)},Ib=()=>{M||(M=function(a,b){this.node=b;this.Ta=function(c){this.la=c};this.Ta(a);this.message="FS error"},M.prototype=Error(),M.prototype.constructor=M,[44].forEach(a=>{eb[a]=new M(a);eb[a].stack="<generic error, no stack>"}))},Kb,Lb=(a,b)=>{var c=0;a&&(c|=365);b&&(c|=146);return c},W=(a,b,c)=>{a=L("/dev/"+a);var d=
Lb(!!b,!!c);Mb||(Mb=64);var e=Mb++<<8|0;Xa(e,{open:g=>{g.seekable=!1},close:()=>{c&&c.buffer&&c.buffer.length&&c(10)},read:(g,k,l,n)=>{for(var p=0,t=0;t<n;t++){try{var u=b()}catch(D){throw new M(29);}if(void 0===u&&0===p)throw new M(6);if(null===u||void 0===u)break;p++;k[l+t]=u}p&&(g.node.timestamp=Date.now());return p},write:(g,k,l,n)=>{for(var p=0;p<n;p++)try{c(k[l+p])}catch(t){throw new M(29);}n&&(g.node.timestamp=Date.now());return p}});xb(a,d,e)},Mb,X={},tb,Db,Nb={};
function Ob(a,b,c){try{var d=a(b)}catch(e){if(e&&e.node&&L(b)!==L(kb(e.node)))return-54;throw e;}H[c>>2]=d.dev;H[c+4>>2]=0;H[c+8>>2]=d.ino;H[c+12>>2]=d.mode;H[c+16>>2]=d.nlink;H[c+20>>2]=d.uid;H[c+24>>2]=d.gid;H[c+28>>2]=d.rdev;H[c+32>>2]=0;Ca[c+40>>3]=BigInt(d.size);H[c+48>>2]=4096;H[c+52>>2]=d.blocks;H[c+56>>2]=d.atime.getTime()/1E3|0;H[c+60>>2]=0;H[c+64>>2]=d.mtime.getTime()/1E3|0;H[c+68>>2]=0;H[c+72>>2]=d.ctime.getTime()/1E3|0;H[c+76>>2]=0;Ca[c+80>>3]=BigInt(d.ino);return 0}var Pb=void 0;
function Qb(){Pb+=4;return H[Pb-4>>2]}function Z(a){a=P[a];if(!a)throw new M(8);return a}function Rb(a,b,c){function d(n){return(n=n.toTimeString().match(/\(([A-Za-z ]+)\)$/))?n[1]:"GMT"}var e=(new Date).getFullYear(),g=new Date(e,0,1),k=new Date(e,6,1);e=g.getTimezoneOffset();var l=k.getTimezoneOffset();H[a>>2]=60*Math.max(e,l);H[b>>2]=Number(e!=l);a=d(g);b=d(k);a=ya(a);b=ya(b);l<e?(H[c>>2]=a,H[c+4>>2]=b):(H[c>>2]=b,H[c+4>>2]=a)}function Sb(a,b,c){Sb.La||(Sb.La=!0,Rb(a,b,c))}var Tb;
Tb=r?()=>{var a=process.hrtime();return 1E3*a[0]+a[1]/1E6}:()=>performance.now();var Ub={};function Vb(){if(!Wb){var a={USER:"web_user",LOGNAME:"web_user",PATH:"/",PWD:"/",HOME:"/home/web_user",LANG:("object"===typeof navigator&&navigator.languages&&navigator.languages[0]||"C").replace("-","_")+".UTF-8",_:ja||"./this.program"},b;for(b in Ub)void 0===Ub[b]?delete a[b]:a[b]=Ub[b];var c=[];for(b in a)c.push(b+"="+a[b]);Wb=c}return Wb}var Wb;
function nb(a,b,c,d){a||(a=this);this.parent=a;this.qa=a.qa;this.ta=null;this.id=hb++;this.name=b;this.mode=c;this.ia={};this.ja={};this.rdev=d}Object.defineProperties(nb.prototype,{read:{get:function(){return 365===(this.mode&365)},set:function(a){a?this.mode|=365:this.mode&=-366}},write:{get:function(){return 146===(this.mode&146)},set:function(a){a?this.mode|=146:this.mode&=-147}}});Ib();Q=Array(4096);vb(N,"/");T("/tmp");T("/home");T("/home/web_user");
(()=>{T("/dev");Xa(259,{read:()=>0,write:(b,c,d,e)=>e});xb("/dev/null",259);Wa(1280,Za);Wa(1536,$a);xb("/dev/tty",1280);xb("/dev/tty1",1536);var a=Ta();W("random",a);W("urandom",a);T("/dev/shm");T("/dev/shm/tmp")})();(()=>{T("/proc");var a=T("/proc/self");T("/proc/self/fd");vb({qa:()=>{var b=db(a,"fd",16895,73);b.ia={lookup:(c,d)=>{var e=P[+d];if(!e)throw new M(8);c={parent:null,qa:{Ja:"fake"},ia:{readlink:()=>e.path}};return c.parent=c}};return b}},"/proc/self/fd")})();
var Zb={a:function(a,b,c,d){y("Assertion failed: "+B(a)+", at: "+[b?B(b):"unknown filename",c,d?B(d):"unknown function"])},j:function(a,b){try{a=B(a);if(b&-8)var c=-28;else{var d=R(a,{ra:!0}).node;d?(a="",b&4&&(a+="r"),b&2&&(a+="w"),b&1&&(a+="x"),c=a&&S(d,a)?-2:0):c=-44}return c}catch(e){if("undefined"===typeof X||!(e instanceof M))throw e;return-e.la}},L:function(a,b){try{return a=B(a),Ab(a,b),0}catch(c){if("undefined"===typeof X||!(c instanceof M))throw c;return-c.la}},J:function(a){try{return a=
B(a),Bb(a),0}catch(b){if("undefined"===typeof X||!(b instanceof M))throw b;return-b.la}},h:function(a,b){try{var c=P[a];if(!c)throw new M(8);Ab(c.node,b);return 0}catch(d){if("undefined"===typeof X||!(d instanceof M))throw d;return-d.la}},K:function(a){try{var b=P[a];if(!b)throw new M(8);Bb(b.node);return 0}catch(c){if("undefined"===typeof X||!(c instanceof M))throw c;return-c.la}},b:function(a,b,c){Pb=c;try{var d=Z(a);switch(b){case 0:var e=Qb();return 0>e?-28:V(d.path,d.flags,0,e).fd;case 1:case 2:return 0;
case 3:return d.flags;case 4:return e=Qb(),d.flags|=e,0;case 5:return e=Qb(),Ba[e+0>>1]=2,0;case 6:case 7:return 0;case 16:case 8:return-28;case 9:return H[Xb()>>2]=28,-1;default:return-28}}catch(g){if("undefined"===typeof X||!(g instanceof M))throw g;return-g.la}},I:function(a,b){try{var c=Z(a);return Ob(U,c.path,b)}catch(d){if("undefined"===typeof X||!(d instanceof M))throw d;return-d.la}},F:function(a,b,c,d){try{b=B(b);var e=d&256;d&=4096;var g=b;if("/"===g[0])b=g;else{if(-100===a)var k="/";else{var l=
P[a];if(!l)throw new M(8);k=l.path}if(0==g.length){if(!d)throw new M(44);b=k}else b=L(k+"/"+g)}return Ob(e?zb:U,b,c)}catch(n){if("undefined"===typeof X||!(n instanceof M))throw n;return-n.la}},D:function(a,b){try{var c=P[a];if(!c)throw new M(8);if(0===(c.flags&2097155))throw new M(28);Cb(c.node,b);return 0}catch(d){if("undefined"===typeof X||!(d instanceof M))throw d;return-d.la}},B:function(a,b){try{if(0===b)return-28;if(b<G("/")+1)return-68;z("/",h,a,b);return a}catch(c){if("undefined"===typeof X||
!(c instanceof M))throw c;return-c.la}},y:function(){return 0},G:function(a,b){try{return a=B(a),Ob(zb,a,b)}catch(c){if("undefined"===typeof X||!(c instanceof M))throw c;return-c.la}},u:function(a,b){try{return a=B(a),a=L(a),"/"===a[a.length-1]&&(a=a.substr(0,a.length-1)),T(a,b),0}catch(c){if("undefined"===typeof X||!(c instanceof M))throw c;return-c.la}},t:function(a,b,c,d,e,g){try{a:{g<<=12;var k=!1;if(0!==(d&16)&&0!==a%65536)var l=-28;else{if(0!==(d&32)){var n=ab(b);if(!n){l=-48;break a}k=!0}else{var p=
P[e];if(!p){l=-8;break a}var t=g;if(0!==(c&2)&&0===(d&2)&&2!==(p.flags&2097155))throw new M(2);if(1===(p.flags&2097155))throw new M(2);if(!p.ja.xa)throw new M(43);var u=p.ja.xa(p,a,b,t,c,d);n=u.Ra;k=u.za}Nb[n]={Oa:n,Na:b,za:k,fd:e,Qa:c,flags:d,offset:g};l=n}}return l}catch(D){if("undefined"===typeof X||!(D instanceof M))throw D;return-D.la}},s:function(a,b){try{var c=Nb[a];if(0!==b&&c){if(b===c.Na){var d=P[c.fd];if(d&&c.Qa&2){var e=c.flags,g=c.offset,k=h.slice(a,a+b);d&&d.ja.ya&&d.ja.ya(d,k,g,b,e)}Nb[a]=
null;c.za&&Yb(c.Oa)}var l=0}else l=-28;return l}catch(n){if("undefined"===typeof X||!(n instanceof M))throw n;return-n.la}},r:function(a,b,c){Pb=c;try{var d=B(a),e=c?Qb():0;return V(d,b,e).fd}catch(g){if("undefined"===typeof X||!(g instanceof M))throw g;return-g.la}},o:function(a,b,c){try{a=B(a);if(0>=c)var d=-28;else{var e=jb(a),g=Math.min(c,G(e)),k=A[b+g];z(e,h,b,c+1);A[b+g]=k;d=g}return d}catch(l){if("undefined"===typeof X||!(l instanceof M))throw l;return-l.la}},n:function(a){try{a=B(a);var b=
R(a,{parent:!0}).node,c=Sa(a),d=O(b,c),e=rb(b,c,!0);if(e)throw new M(e);if(!b.ia.rmdir)throw new M(63);if(d.ta)throw new M(10);b.ia.rmdir(b,c);mb(d);return 0}catch(g){if("undefined"===typeof X||!(g instanceof M))throw g;return-g.la}},H:function(a,b){try{return a=B(a),Ob(U,a,b)}catch(c){if("undefined"===typeof X||!(c instanceof M))throw c;return-c.la}},m:function(a){try{return a=B(a),fa(a),0}catch(b){if("undefined"===typeof X||!(b instanceof M))throw b;return-b.la}},e:function(){y("To use dlopen, you need to use Emscripten's linking support, see https://github.com/emscripten-core/emscripten/wiki/Linking")},
i:function(){y("To use dlopen, you need to use Emscripten's linking support, see https://github.com/emscripten-core/emscripten/wiki/Linking")},w:function(a,b){a=new Date(1E3*H[a>>2]);H[b>>2]=a.getSeconds();H[b+4>>2]=a.getMinutes();H[b+8>>2]=a.getHours();H[b+12>>2]=a.getDate();H[b+16>>2]=a.getMonth();H[b+20>>2]=a.getFullYear()-1900;H[b+24>>2]=a.getDay();var c=new Date(a.getFullYear(),0,1);H[b+28>>2]=(a.getTime()-c.getTime())/864E5|0;H[b+36>>2]=-(60*a.getTimezoneOffset());var d=(new Date(a.getFullYear(),
6,1)).getTimezoneOffset();c=c.getTimezoneOffset();H[b+32>>2]=(d!=c&&a.getTimezoneOffset()==Math.min(c,d))|0},x:Sb,k:function(){y("")},g:Tb,q:function(a,b,c){h.copyWithin(a,b,b+c)},d:function(a){var b=h.length;a>>>=0;if(2147483648<a)return!1;for(var c=1;4>=c;c*=2){var d=b*(1+.2/c);d=Math.min(d,a+100663296);d=Math.max(a,d);0<d%65536&&(d+=65536-d%65536);a:{try{ra.grow(Math.min(2147483648,d)-Aa.byteLength+65535>>>16);Da();var e=1;break a}catch(g){}e=void 0}if(e)return!0}return!1},z:function(a,b){var c=
0;Vb().forEach(function(d,e){var g=b+c;e=H[a+4*e>>2]=g;for(g=0;g<d.length;++g)A[e++>>0]=d.charCodeAt(g);A[e>>0]=0;c+=d.length+1});return 0},A:function(a,b){var c=Vb();H[a>>2]=c.length;var d=0;c.forEach(function(e){d+=e.length+1});H[b>>2]=d;return 0},c:function(a){try{var b=Z(a);Eb(b);return 0}catch(c){if("undefined"===typeof X||!(c instanceof M))throw c;return c.la}},l:function(a,b){try{var c=Z(a);A[b>>0]=c.tty?2:16384===(c.mode&61440)?3:40960===(c.mode&61440)?7:4;return 0}catch(d){if("undefined"===
typeof X||!(d instanceof M))throw d;return d.la}},p:function(a,b,c,d){try{a:{for(var e=Z(a),g=a=0;g<c;g++){var k=H[b+(8*g+4)>>2],l=Gb(e,A,H[b+8*g>>2],k,void 0);if(0>l){var n=-1;break a}a+=l;if(l<k)break}n=a}H[d>>2]=n;return 0}catch(p){if("undefined"===typeof X||!(p instanceof M))throw p;return p.la}},v:function(a,b,c,d){try{var e=Number(b&BigInt(4294967295))|0,g=Number(b>>BigInt(32))|0,k=Z(a);a=4294967296*g+(e>>>0);if(-9007199254740992>=a||9007199254740992<=a)return-61;Fb(k,a,c);Ca[d>>3]=BigInt(k.position);
k.Ba&&0===a&&0===c&&(k.Ba=null);return 0}catch(l){if("undefined"===typeof X||!(l instanceof M))throw l;return l.la}},E:function(a){try{var b=Z(a);return b.ja&&b.ja.fsync?-b.ja.fsync(b):0}catch(c){if("undefined"===typeof X||!(c instanceof M))throw c;return c.la}},f:function(a,b,c,d){try{a:{for(var e=Z(a),g=a=0;g<c;g++){var k=Hb(e,A,H[b+8*g>>2],H[b+(8*g+4)>>2]);if(0>k){var l=-1;break a}a+=k}l=a}H[d>>2]=l;return 0}catch(n){if("undefined"===typeof X||!(n instanceof M))throw n;return n.la}},C:function(a){var b=
Date.now();H[a>>2]=b/1E3|0;H[a+4>>2]=b%1E3*1E3|0;return 0},M:function(a){var b=Date.now()/1E3|0;a&&(H[a>>2]=b);return b},N:function(a,b){if(b){var c=b+8;b=1E3*H[c>>2];b+=H[c+4>>2]/1E3}else b=Date.now();a=B(a);try{var d=R(a,{ra:!0}).node;d.ia.na(d,{timestamp:Math.max(b,b)});var e=0}catch(g){if(!(g instanceof M)){b:{e=Error();if(!e.stack){try{throw Error();}catch(k){e=k}if(!e.stack){e="(no stack trace available)";break b}}e=e.stack.toString()}f.extraStackTrace&&(e+="\n"+f.extraStackTrace());e=Pa(e);
throw g+" : "+e;}e=g.la;H[Xb()>>2]=e;e=-1}return e}};
(function(){function a(e){f.asm=e.exports;ra=f.asm.O;Da();Ea=f.asm.da;Ga.unshift(f.asm.P);I--;f.monitorRunDependencies&&f.monitorRunDependencies(I);0==I&&(null!==Ja&&(clearInterval(Ja),Ja=null),J&&(e=J,J=null,e()))}function b(e){a(e.instance)}function c(e){return Na().then(function(g){return WebAssembly.instantiate(g,d)}).then(function(g){return g}).then(e,function(g){w("failed to asynchronously prepare wasm: "+g);y(g)})}var d={a:Zb};I++;f.monitorRunDependencies&&f.monitorRunDependencies(I);if(f.instantiateWasm)try{return f.instantiateWasm(d,
a)}catch(e){return w("Module.instantiateWasm callback failed with error: "+e),!1}(function(){return x||"function"!==typeof WebAssembly.instantiateStreaming||Ka()||K.startsWith("file://")||"function"!==typeof fetch?c(b):fetch(K,{credentials:"same-origin"}).then(function(e){return WebAssembly.instantiateStreaming(e,d).then(b,function(g){w("wasm streaming compile failed: "+g);w("falling back to ArrayBuffer instantiation");return c(b)})})})();return{}})();
f.___wasm_call_ctors=function(){return(f.___wasm_call_ctors=f.asm.P).apply(null,arguments)};var ba=f._sqlite3_malloc=function(){return(ba=f._sqlite3_malloc=f.asm.Q).apply(null,arguments)},da=f._sqlite3_free=function(){return(da=f._sqlite3_free=f.asm.R).apply(null,arguments)};f._sqlite3_errmsg=function(){return(f._sqlite3_errmsg=f.asm.S).apply(null,arguments)};
var ha=f._sqlite3_initialize=function(){return(ha=f._sqlite3_initialize=f.asm.T).apply(null,arguments)},Xb=f.___errno_location=function(){return(Xb=f.___errno_location=f.asm.U).apply(null,arguments)},za=f._malloc=function(){return(za=f._malloc=f.asm.V).apply(null,arguments)},Yb=f._free=function(){return(Yb=f._free=f.asm.W).apply(null,arguments)};f.___dbFileImportOrExport=function(){return(f.___dbFileImportOrExport=f.asm.X).apply(null,arguments)};
f._dbClose=function(){return(f._dbClose=f.asm.Y).apply(null,arguments)};f._dbExec=function(){return(f._dbExec=f.asm.Z).apply(null,arguments)};f._jsbatonValueStringArgi=function(){return(f._jsbatonValueStringArgi=f.asm._).apply(null,arguments)};f._dbFileImportOrExport=function(){return(f._dbFileImportOrExport=f.asm.$).apply(null,arguments)};f._dbNoop=function(){return(f._dbNoop=f.asm.aa).apply(null,arguments)};f._dbOpen=function(){return(f._dbOpen=f.asm.ba).apply(null,arguments)};
f._jsbatonValueErrmsg=function(){return(f._jsbatonValueErrmsg=f.asm.ca).apply(null,arguments)};var bb=f._memalign=function(){return(bb=f._memalign=f.asm.ea).apply(null,arguments)},va=f.stackSave=function(){return(va=f.stackSave=f.asm.fa).apply(null,arguments)},wa=f.stackRestore=function(){return(wa=f.stackRestore=f.asm.ga).apply(null,arguments)},ua=f.stackAlloc=function(){return(ua=f.stackAlloc=f.asm.ha).apply(null,arguments)};f.cwrap=m;var $b;J=function ac(){$b||bc();$b||(J=ac)};
function bc(){function a(){if(!$b&&($b=!0,f.calledRun=!0,!sa)){f.noFSInit||Kb||(Kb=!0,Ib(),f.stdin=f.stdin,f.stdout=f.stdout,f.stderr=f.stderr,f.stdin?W("stdin",f.stdin):yb("/dev/tty","/dev/stdin"),f.stdout?W("stdout",null,f.stdout):yb("/dev/tty","/dev/stdout"),f.stderr?W("stderr",null,f.stderr):yb("/dev/tty1","/dev/stderr"),V("/dev/stdin",0),V("/dev/stdout",1),V("/dev/stderr",1));ib=!1;Oa(Ga);if(f.onRuntimeInitialized)f.onRuntimeInitialized();if(f.postRun)for("function"==typeof f.postRun&&(f.postRun=
[f.postRun]);f.postRun.length;){var b=f.postRun.shift();Ha.unshift(b)}Oa(Ha)}}if(!(0<I)){if(f.preRun)for("function"==typeof f.preRun&&(f.preRun=[f.preRun]);f.preRun.length;)Ia();Oa(Fa);0<I||(f.setStatus?(f.setStatus("Running..."),setTimeout(function(){setTimeout(function(){f.setStatus("")},1);a()},1)):a())}}f.run=bc;if(f.preInit)for("function"==typeof f.preInit&&(f.preInit=[f.preInit]);0<f.preInit.length;)f.preInit.pop()();bc();

}());
/*jslint-enable*/