package com.crylent.midilib.envelope

@Suppress("MemberVisibilityCanBePrivate")
class ADSREnvelope(
    attack: Number = 0f,
    decay: Number = 5f,
    sustain: Number = 0f,
    release: Number = 0f,
    attackSharpness: Number = 1f,
    decaySharpness: Number = 1f,
    releaseSharpness: Number = 1f
): ReleaseOnlyEnvelope(release, releaseSharpness) {
    override fun getType() = Type.ADSR

    var attack = attack.toFloat()
        set(value) {
            field = value
            externalSetAttack(value)
        }

    var decay = decay.toFloat()
        set(value) {
            field = value
            externalSetDecay(value)
        }

    var sustain = sustain.toFloat()
        set(value) {
            field = value
            externalSetSustain(value)
        }

    var attackSharpness = attackSharpness.toFloat()
        set(value) {
            field = value
            externalSetAttackSharpness(value)
        }

    var decaySharpness = decaySharpness.toFloat()
        set(value) {
            field = value
            externalSetDecaySharpness(value)
        }

    private external fun externalSetAttack(value: Float)
    private external fun externalSetDecay(value: Float)
    private external fun externalSetSustain(value: Float)
    private external fun externalSetAttackSharpness(value: Float)
    private external fun externalSetDecaySharpness(value: Float)

    override fun clone() = ADSREnvelope(
        attack, decay, sustain, release,
        attackSharpness, decaySharpness, releaseSharpness
    )
}