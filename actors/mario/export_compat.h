#ifndef ACTORS_MARIO_EXPORT_COMPAT_H
#define ACTORS_MARIO_EXPORT_COMPAT_H

// Fast64 Mario exports can reference a Fresnel display-list macro and a couple
// of layer-5 eye materials that this codebase does not define. Keep the
// compatibility here instead of patching generated files so re-exports do not
// break the build.

#ifndef gsSPFresnel
#define gsSPFresnel(a, b) gsDPNoOp()
#endif

#define mat_mario_face_1___eye_half_v4_001_layer5 mat_mario_face_1___eye_half_v4_001_layer1
#define mat_mario_face_2___eye_closed_v4_001_layer5 mat_mario_face_2___eye_closed_v4_001_layer1

#endif // ACTORS_MARIO_EXPORT_COMPAT_H