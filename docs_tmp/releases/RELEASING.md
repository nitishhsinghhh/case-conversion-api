# Release Process & Versioning Logic

This document outlines the procedure for promoting the Case Conversion API through version milestones.

## 1. Versioning Strategy (SemVer)

We follow Semantic Versioning (`MAJOR.MINOR.PATCH`):

- **MAJOR:** Significant architectural shifts (e.g., v2.0.0 NGINX Cluster migration).
- **MINOR:** Performance milestones or new native strategies (e.g., 1M Request validation).
- **PATCH:** Bug fixes or CI/CD workflow tweaks.

## 2. Release Procedure

To trigger a Production Release on the ARM64 macOS runner:

1. **Update Release Notes:** Modify the `body` section in `.github/workflows/release.yml`.
2. **Commit Changes:** Ensure all documentation and core logic are in `main`.
3. **Tag & Push:**

```bash
   git tag -a v2.0.0 -m "Major Release: HA Architecture & 1M Milestone"
   git push origin v2.0.0
```

## 3. Hardware Alignment

Every release is compiled on macos-latest to ensure the C++ .dylib is optimized for Apple Silicon (M-Series) Performance Cores.

---
