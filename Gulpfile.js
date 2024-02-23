const fs = require('fs');
const crypto = require('crypto');
const { series, src, dest, watch, parallel } = require('gulp');
const del = require('delete');
const htmlmin = require('gulp-htmlmin');
const cleancss = require('gulp-clean-css');
const favicon = require('gulp-base64-favicon');
const inline = require('gulp-inline');
const uglify = require('gulp-uglify');
const gzip = require('gulp-gzip');
const gulpEsbuild = require('gulp-esbuild')
const inlineImages = require('gulp-css-base64');
const size = require('gulp-size');
const connect = require('gulp-connect');
const { createProxyMiddleware } = require('http-proxy-middleware');

const isProduction = process.env.NODE_ENV == 'production';

function cleanup(cb) {
  del([
    'tmp/output/**/*.*',
    'tmp/static/**/*.*',
    'tmp/inline/**/*.*',
    // 'include/*.html.gz.h'
  ], cb);
}

function justCopyHtmlForTest() {
  return src(["web/index.html", "web/*.svg"])
    .pipe(dest('tmp/output'))
}

function generateJs() {
  return src(["web/src/index.tsx"])
    .pipe(gulpEsbuild({
      bundle: true,
      incremental: !isProduction,
      sourcemap: isProduction ? false : 'inline', //TODO: if development
      minify: isProduction,
      legalComments: 'none',
      minifyWhitespace: isProduction,
      minifyIdentifiers: isProduction,
      minifySyntax: isProduction,
      treeShaking: true,

      loader: {
        '.tsx': 'tsx',
        '.mp3': 'dataurl'
      },
  }))
  .pipe(dest('tmp/output'))
}

// Joins all file into one gziped html file
function createOneFile() {
  return src('tmp/output/**/*.html')
    .pipe(favicon())
    .pipe(inline({
      base: "./output/inline",
      js: uglify,
      css: [cleancss, inlineImages],
      disabledTypes: ["js", "svg"]
    }))
    .pipe(htmlmin({
      collapseWhitespace: true,
      removecomments: true,
      aside: true,
      minifyCSS: true,
      minifyJS: true
    }))
    .pipe(dest("tmp/inline"));
}

function compressFile() {
  return src(['tmp/inline/index.html', 'tmp/output/index.js', 'tmp/output/*.svg'])
  .pipe(gzip({
    gzipOptions: { level: 9 }
  }))
  .pipe(dest("tmp/inline"));
}

function buildGenerateCppHeaderWithEncodedContent(fileName) {
  return (cb) => {
    const variable = fileName.replace(".", "_");
    var output = fs.createWriteStream(`include/${fileName}.gz.h`);
    output.on('error', function (err) {
      console.log(err);
    });

    var input = fs.readFileSync(`tmp/inline/${fileName}.gz`);
    let hash = crypto.createHash('md5').update(input).digest("hex")

    // TODO: generate etag by md5
    output.write(`#define ${variable}_gz_len ` + input.length + '\n');
    output.write(`const char * ${variable}_gz_hash = "` + hash + '";\n');
    output.write(`const uint8_t ${variable}_gz[] PROGMEM = {`)

    for (i=0; i<input.length; i++) {
      if (i % 1000 == 0) output.write("\n");

      output.write('0x' + ('00' + input[i].toString(16)).slice(-2));

      if (i<input.length-1) {
        output.write(',');
      }
    }

    output.write('\n};')
    output.end();

    cb()
  }
}

function printSummary() {
  const s = size({
    title: "Bundle size",
    pretty: true,
  });

  return src(['tmp/inline', 'tmp/output'])
		.pipe(s)
		.pipe(dest('tmp/'))
}

function server() {
  connect.server({
    root: 'tmp/output',
    livereload: true,
    debug: true,
    fallback: "tmp/output/index.html",
    middleware: (connect, opt, server) => {
      const apiProxy = createProxyMiddleware({
        target: 'http://192.168.10.146/',
        changeOrigin: true,
        ws: true
      });

      return [
        [
          "/api", apiProxy,
        ]
      ]
    }
  });
}

const buildHtmlHeaderFile = buildGenerateCppHeaderWithEncodedContent("index.html");
const buildJavascriptHeaderFile = buildGenerateCppHeaderWithEncodedContent("index.js");
const dotsSVG = buildGenerateCppHeaderWithEncodedContent("dots.svg");
const zaku3SVG = buildGenerateCppHeaderWithEncodedContent("zaku3.svg");

exports.build = series(generateJs, justCopyHtmlForTest);
exports.cleanBuild = series(cleanup, exports.build);
exports.release = series(exports.cleanBuild, createOneFile, compressFile, dotsSVG, zaku3SVG, buildHtmlHeaderFile, buildJavascriptHeaderFile);

function dev () {
  watch('web/**/*.{js,tsx,jsx,html,ts,css,svg}', exports.build);
}

exports.default = series(exports.cleanBuild, exports.build, parallel(server, dev));
