package com.crylent.midilib.envelope

open class ReleaseOnlyEnvelope(
    release: Number = 0f,
    releaseSharpness: Number = 1f
): Envelope() {
    override fun getType() = Type.ReleaseOnly

    var release = release.toFloat()
        set(value) {
            field = value
            externalSetRelease(value)
        }

    var releaseSharpness = releaseSharpness.toFloat()
        set(value) {
            field = value
            externalSetReleaseSharpness(value)
        }

    private external fun externalSetRelease(value: Float)
    private external fun externalSetReleaseSharpness(value: Float)

    override fun clone() = ReleaseOnlyEnvelope(release, releaseSharpness)
}