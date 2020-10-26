#!/bin/sh -l

echo "-------------------------------------------"
echo "Start style linting for all Markdown files."
echo "-------------------------------------------"

markdownlint . -c .github/actions/markdownlint/.markdownlintrc

EXITCODE=$?
SUCCESS_MESSAGE="[OK]Everything looks perfect."
ERROR_MESSAGE="[ERROR]Something is wrong."

test ${EXITCODE} -eq 0 && echo ${SUCCESS_MESSAGE} || echo ${ERROR_MESSAGE};
exit ${EXITCODE}
