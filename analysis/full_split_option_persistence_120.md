# RE5DX9 1.2.0 — SplitModeQuality / Full split persistence

Direct authority: unpacked 1.2.0 executable SHA-256 `1c2d07d4d94c3d72833700f4fef114c7320b855e83e205e30df8b14186ec8cd8`.

## Exact native setting identity

The 11-entry graphics/system option-name table at `0x10E3F38` resolves as:

0. Resolution
1. FullScreen
2. RefreshRate
3. VSYNC
4. FrameRate
5. MSAA
6. MotionBlurQuality
7. ShadowQuality
8. TextureDetail
9. ScreenQuality
10. **SplitModeQuality** (`0xF5A034`)

Therefore `root + 0x104D0 + index*4` gives the cached selected index for each setting, and slot 10 is exactly `root+0x104F8`.

## Getter/setter backing field

`0x77EAD0`:

- `mov eax,[ecx+0x10540]`
- `ret`

`0x77EAE0`:

- loads the sole stack argument
- stores it to `[ecx+0x10540]`
- `ret 4`

The registration block at `0x781695..0x78171E` names the setting `SplitModeQuality` and installs callbacks `0x77EAD0` / `0x77EAE0`.

## Serialized field

`0x782D72..0x782DB1` constructs a serializer/property descriptor using exact field address `self+0x10540` and key string `mSplitModeQuality` at `0xF5F824`, then calls the generic registration helper `0x422C00`.

This closes the persistence backing path: the actual saved configuration field is `+0x10540`.

## Runtime load to split state

At the tail of `0x780CA0`, after all 11 settings are resolved, `0x780EC0..0x780ED6` executes:

- compare `[root+0x104F8]` with `1`
- `sete al`
- load global split state `0x123457C`
- store `al` to `[split+0x3084]`

So native `fullScreenSplitMode` is exactly `(cached SplitModeQuality selection == 1)`.

## Live menu change

`0x780F10` iterates the same 11 settings. Its jump table at `0x781344` maps option index 10 to `0x781210`.

The slot-10 handler commits the enum value, then at `0x78123B..0x781254`:

- loads global split state `0x123457C`
- calls `0x424F30` on the SplitModeQuality menu object
- compares returned value with `1`
- stores the equality result to `[split+0x3084]`

Then `0x781254..0x781274` queries runtime status through `0xC42D90`; when status is `1`, it calls `0x76CC10(split)` immediately to rebuild/update the split presentation.

## Conclusion

The menu option and gameplay flag are not separate independent settings in native 1.2.0. `SplitModeQuality` is the registered/persisted graphics option; its selected index/value drives `split+0x3084`, and changing it while in status 1 immediately triggers the split UI/runtime rebuild path.
