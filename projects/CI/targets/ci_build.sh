#!/bin/bash
#
#  AUTO GENERATED BY PROJECTGENERATOR CI HELPER
#

# Provide easy access to the location of the script dir itself
DIR="$(cd "${0%/*}" 2>/dev/null; echo "$PWD"/"${0##*/}")"

# Provide the environment variables for troubleshooting
echo -e "TARGET_NAME: $TARGET_NAME\nTARGET_PLATFORM: $TARGET_PLATFORM\nTARGET_PROJECT: $TARGET_PROJECT\nPRODUCT_NAME: $PRODUCT_NAME\nCI_TOOL: $CI_TOOL\nBUILD_TOOL: $BUILD_TOOL\nBUILD_TYPE: $BUILD_TYPE\n"

# Provide the CI tooling specific environment variables for troubleshooting
if [[ ${CI_TOOL} == "GITHUB-ACTIONS" ]]; then
    echo -e "CI: $CI\nGITHUB_WORKFLOW: $GITHUB_WORKFLOW\nGITHUB_RUN_ID: $GITHUB_RUN_ID\nGITHUB_RUN_NUMBER: $GITHUB_RUN_NUMBER\nGITHUB_JOB: $GITHUB_JOB\nGITHUB_ACTION: $GITHUB_ACTION\nGITHUB_ACTION_PATH: $GITHUB_ACTION_PATH\nGITHUB_ACTIONS: $GITHUB_ACTIONS\nGITHUB_ACTOR: $GITHUB_ACTOR\nGITHUB_REPOSITORY: $GITHUB_REPOSITORY\nGITHUB_EVENT_NAME: $GITHUB_EVENT_NAME\nGITHUB_EVENT_PATH: $GITHUB_EVENT_PATH\nGITHUB_WORKSPACE: $GITHUB_WORKSPACE\nGITHUB_SHA: $GITHUB_SHA\nGITHUB_REF: $GITHUB_REF\nGITHUB_HEAD_REF: $GITHUB_HEAD_REF\nGITHUB_BASE_REF: $GITHUB_BASE_REF\nGITHUB_SERVER_URL: $GITHUB_SERVER_URL\nGITHUB_API_URL: $GITHUB_API_URL\nGITHUB_GRAPHQL_URL: $GITHUB_GRAPHQL_URL\nRUNNER_OS: $RUNNER_OS\nRUNNER_TOOL_CACHE: $RUNNER_TOOL_CACHE\n"
    REPO_ROOT=$GITHUB_WORKSPACE
else
    echo "Unknown CI Tooling"
fi


if [[ -z "$BUILD_TOOL" ]]; then
    echo "Build tool was not specified. Will default to CMake"
    BUILD_TOOL="CMAKE"
fi


if [[ -z "$BUILD_TYPE" ]]; then
    echo "Build type was not specified. Will default to 'release'"
    BUILD_TYPE="release"
fi


if [[ ${BUILD_TOOL} == "CMAKE" ]]; then
    cmake -H"$REPO_ROOT/$TARGET_PROJECT" -B"$REPO_ROOT/common/bin/$TARGET_NAME-$TARGET_PLATFORM" -DCMAKE_BUILD_TYPE=$BUILD_TYPE
    cmake --build "$REPO_ROOT/common/bin/$TARGET_NAME-$TARGET_PLATFORM"
    cmake --install "$REPO_ROOT/common/bin/$TARGET_NAME-$TARGET_PLATFORM" --prefix instdir --strip
    cpack --config "$REPO_ROOT/common/bin/$TARGET_NAME-$TARGET_PLATFORM/CPackConfig.cmake"
else
    echo "Unknown Build tool"
fi

