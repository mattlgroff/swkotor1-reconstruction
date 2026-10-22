# swkotor1-reconstruction

This project reconstructs the **GOG Windows 1.03** version of **Star Wars: Knights of the Old Republic** from its executable. This repository is intended to house the reconstructed source code. You must purchase the original game and supply its assets.

![Reconstruction progress: playable build and MSVC 7.0 matching track](matching/assets/progress.svg)

This project has two decompilations of the same executable:

- `src/`: built with modern Visual Studio 2022 and its libraries. Work of [mattlgroff](https://github.com/mattlgroff).
- `matching/`: built with the original Visual Studio .NET 2002 and its libraries, so it is byte-identical. Work of [RayesDiyab](https://github.com/RayesDiyab).

See [STRUCTURE.md](STRUCTURE.md). Milestones follow [STATUS.md](STATUS.md).

## Supported version

The project targets the English **GOG Windows 1.03** release. The original `swkotor.exe` is a 32-bit executable with file and product version **1.0.3.0** and a size of **4,042,752 bytes**.

Original executable SHA-256:

```text
9c10e0450a6eeca417e036e3cde7474fed1f0a92aab018446d156944dea91435
```

Use this hash to identify the target executable. Other releases and modified executables may differ.

## Original game required

You must separately purchase and lawfully possess the original game and supply its assets locally. This repository distributes no original game resources, proprietary binaries, middleware, or raw decompiler exports.

See [BUILDING.md](BUILDING.md) for build instructions, [STATUS.md](STATUS.md) for current progress and [CONTRIBUTING.md](CONTRIBUTING.md) for bug reports and source contributions.

## Rights and affiliation

This project is unofficial and is not affiliated with or endorsed by the game's creators, publishers, GOG, or other rights holders.

The original game, its trademarks, assets, and other third-party materials remain the property of their respective rights holders. All rights reserved by the respective rights holders. This project does not claim ownership of those rights or grant permission to redistribute proprietary game materials.

The project uses the [MIT License](LICENSE) for any rights its contributors hold. This does not grant rights to original KOTOR code, assets, trademarks, or other third-party material.
