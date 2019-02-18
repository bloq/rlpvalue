
# TO-DO

## Technical debt

* univalue included as a git subtree because of difficulty in
  installing dependencies across all travis test platforms.
  Ideally:  remove submodule, discover via configure.

* win64 build disabled in travis due to lack of argp-standalone
  on that platform.  find argp-standalone, and re-enable win64.

## Low priority

Rearrange tree for easier 'git subtree' style use

Namespace support - must come up with useful shorthand, avoiding
long Univalue::Univalue::Univalue usages forced upon library users.

Improve test suite

