#!/usr/bin/env bash

# -----------------------------------------------------------------------------
# Safety settings (see https://gist.github.com/ilg-ul/383869cbb01f61a51c4d).

if [[ ! -z ${DEBUG} ]]
then
  set ${DEBUG} # Activate the expand mode if DEBUG is -x.
else
  DEBUG=""
fi

set -o errexit # Exit if command failed.
set -o pipefail # Exit if pipe failed.
set -o nounset # Exit if variable not set.

# Remove the initial space and instead use '\n'.
IFS=$'\n\t'

# ------------------------------------------------------------------

# This is a temporary solution, until a full template substitution
# command using liquid tags and filters will be available.
# For now the accepted syntax is very strict, to simplify the sed expressions.

template_source="templates/version.liquid.h"
template_destination="include/segger-jlink-rtos-plugin-sdk/version.h"

# This simple code does not support prereleases.
xpack_name=$(echo ${npm_package_name} | sed -e "s/@//" | tr '[:lower:]' '[:upper:]' | tr '/-' '_' )
xpack_version_major=$(echo ${npm_package_version} | sed -e "s/\([0-9]*\)\.\([0-9]*\)\.\([0-9]*\).*/\1/") 
xpack_version_minor=$(echo ${npm_package_version} | sed -e "s/\([0-9]*\)\.\([0-9]*\)\.\([0-9]*\).*/\2/")
xpack_version_patch=$(echo ${npm_package_version} | sed -e "s/\([0-9]*\)\.\([0-9]*\)\.\([0-9]*\).*/\3/")

mkdir -p "$(dirname ${template_destination})"
sed \
-e "/{% assign/d" \
-e "s|{{ template.source }}|${template_source}|" \
-e "s/{{ macro_name }}/${xpack_name}/" \
-e "s/{{ xpack.version.major }}/${xpack_version_major}/" \
-e "s/{{ xpack.version.minor }}/${xpack_version_minor}/" \
-e "s/{{ xpack.version.patch }}/${xpack_version_patch}/" \
"${template_source}" >"${template_destination}"

git add -A "${template_destination}"

