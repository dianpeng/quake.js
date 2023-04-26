#!/bin/sh

mkdir -p coverage

gcovr -e "src/control/api/\.*"             \
      -e "src/url/chromium-url/\.*"        \
      -e "src/urlpattern/chromium-url/\.*" \
      -e "src/debugger/\.*"                \
      -e "include/debugger/\.*"            \
      -e "src/js/gen/\.*"                  \
      -e "src/shell/\.*"                   \
      -e "src/sandbox\.*"                  \
      -e "unittest/\.*"                    \
      -e "third-party/\.*"                 \
      -r ./                                \
      --html --html-details -o coverage/coverage.html
