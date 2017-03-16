#!/bin/bash

# print new line in shell script,  echo can't do this
cat >&2 << __EOF__
Could not find source code.  Make sure you are running this script from the
root of the distribution tree.
__EOF__

# update the shell script input para. e.g. add a new para before old $@
set -- "add a new para before" "$@"_

