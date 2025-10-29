Junkcoin Core integration/staging tree
=====================================

⚠️ **WARNING: NOT PRODUCTION READY** ⚠️
-------------------------------------------
**This software is currently under active development and is NOT ready for production use.**
**Do NOT use this for mainnet operations or with real funds.**
**This is a development fork of Litecoin being adapted for Junkcoin.**

https://junk-coin.com

What is Junkcoin?
----------------

Junkcoin is an experimental digital currency that enables instant payments to
anyone, anywhere in the world. Junkcoin uses peer-to-peer technology to operate
with no central authority: managing transactions and issuing money are carried
out collectively by the network. Junkcoin Core is the name of open source
software which enables the use of this currency.

For more information, as well as an immediately useable, binary version of
the Junkcoin Core software, see [https://junk-coin.com](https://junk-coin.com).

History
-------

Junkcoin was originally forked from Litecoin in its early development. Over time,
the codebase diverged as Junkcoin implemented its own features and modifications.
In 2025, the project underwent a significant migration to return to its Litecoin
lineage while preserving all Junkcoin-specific features and improvements.

This migration involved:
- Returning to the clean Litecoin codebase as the foundation
- Merging all Junkcoin-specific modifications (AuxPoW, custom fee structures, branding)
- Updating dependencies for modern compatibility
- Establishing a clean foundation for future Junkcoin development

By returning to the Litecoin lineage, Junkcoin benefits from:
- Better compatibility with Litecoin ecosystem tools and libraries
- Easier integration of upstream Litecoin improvements and security fixes
- A well-tested and battle-hardened codebase foundation
- Clear separation between core Litecoin functionality and Junkcoin innovations

License
-------

Junkcoin Core is released under the terms of the MIT license. See [COPYING](COPYING) for more
information or see https://opensource.org/licenses/MIT.

Development Process
-------------------

The `master` branch is regularly built (see `doc/build-*.md` for instructions) and tested, but it is not guaranteed to be
completely stable. This is an active development fork and should be considered unstable.

The contribution workflow is described in [CONTRIBUTING.md](CONTRIBUTING.md)
and useful hints for developers can be found in [doc/developer-notes.md](doc/developer-notes.md).

Testing
-------

Testing and code review is the bottleneck for development; we get more pull
requests than we can review and test on short notice. Please be patient and help out by testing
other people's pull requests, and remember this is a security-critical project where any mistake might cost people
lots of money.

### Automated Testing

Developers are strongly encouraged to write [unit tests](src/test/README.md) for new code, and to
submit new unit tests for old code. Unit tests can be compiled and run
(assuming they weren't disabled in configure) with: `make check`. Further details on running
and extending unit tests can be found in [/src/test/README.md](/src/test/README.md).

There are also [regression and integration tests](/test), written
in Python, that are run automatically on the build server.
These tests can be run (if the [test dependencies](/test) are installed) with: `test/functional/test_runner.py`

The Travis CI system makes sure that every pull request is built for Windows, Linux, and macOS, and that unit/sanity tests are run automatically.

### Manual Quality Assurance (QA) Testing

Changes should be tested by somebody other than the developer who wrote the
code. This is especially important for large or high-risk changes. It is useful
to add a test plan to the pull request description if testing the changes is
not straightforward.

Translations
------------

Translation support will be added in future releases. Currently, translations are not being actively managed for Junkcoin.
