#!/bin/bash

declare -a modules=(
    "JaVaSTyle"
    "JavaFXExamples"
    "jvst_examples"
    "jvst_native"
    "jvst_system"
    "jvst_test"
    "jvst_wrapper"
)

module="$1"

echo "Starting $module"
git remote add "$module" ~/multi-module/"$module"
git fetch "$module" --tags
git merge --allow-unrelated-histories "$module"/master
git remote remove "$module"
mkdir "$module"
