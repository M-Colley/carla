# Reproducible CARLA client validation source

This branch gathers the production and regression-test changes from Mark
Colley's nine separate `ue58-dev` pull requests. It starts at upstream
`defabe35d1ad47c4231d4f7fb1f4e46ac2e08ec5`. The source files match these reviewed
branch revisions:

| PR | Revision | Scope |
| --- | --- | --- |
| [9991](https://github.com/carla-simulator/carla/pull/9991) | `d8256f6` | Windows setup reuse/content/Python paths |
| [9992](https://github.com/carla-simulator/carla/pull/9992) | `73ba60d` | Python gear-ratio conversions |
| [9993](https://github.com/carla-simulator/carla/pull/9993) | `faaf4b2` | Empty actor snapshot initialization |
| [9994](https://github.com/carla-simulator/carla/pull/9994) | `3235edc` | Native wheel backend, paths and Boost selection |
| [9995](https://github.com/carla-simulator/carla/pull/9995) | `23ac207` | Atomic compare-exchange failure ordering |
| [9996](https://github.com/carla-simulator/carla/pull/9996) | `d819bb5` | Future waiter lifetime, publication cost, lvalue exceptions |
| [9997](https://github.com/carla-simulator/carla/pull/9997) | `3f45aec` | Streaming listener/ROS synchronization |
| [9998](https://github.com/carla-simulator/carla/pull/9998) | `b188eb3` | Native/Python unit-test portability |
| [9999](https://github.com/carla-simulator/carla/pull/9999) | `33c94d5` | Episode exception/map synchronization |

This is a validation snapshot, not a merged upstream release. It includes
the source/build/test changes and the standalone benchmark/test instructions;
the individual PRs carry their own changelogs, CI workflows and broader docs.

Use the [fresh-PC guide](https://github.com/M-Colley/myosuite-carla/blob/main/NEW_PC_SETUP.md)
and [validation record](https://github.com/M-Colley/myosuite-carla/blob/main/VALIDATION.md)
for commands, exact results, wheel hashes and environment provenance. The bridge
repository is private to its owner and authorized collaborators.

The native client build uses CPython 3.12, C++20 and Boost 1.90. The recorded
dependency overrides use CARLA rpclib commit
`bbfe141f4b5c1396cedb91071f8a839a6fa9de35`, GoogleTest commit
`52eb8108c5bdec04579160ae17225d66034bd723`, and OpenDRIVE test fixtures
`5929b67cc414b611b701ad251fe1e6d30f465bba` (tag `0.1.4`).

Rebuild LibCarla and all consumers together: some private class layouts change.
The standalone client and Python extension can be built without Unreal Engine
or simulator content. A successful client build/unit test does not establish
that an Unreal Engine 5.8 simulator build or vehicle-physics run has passed.
Live bridge results recorded so far use the separately identified UE5.5 release.
