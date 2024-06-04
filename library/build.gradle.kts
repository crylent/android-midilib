@file:Suppress("UnstableApiUsage")

plugins {
    alias(libs.plugins.androidLibrary) apply true
    alias(libs.plugins.jetbrainsKotlinAndroid) apply true
    `maven-publish` apply true
}

publishing {
    publications {
        register<MavenPublication>("release") {
            groupId = "com.crylent"
            artifactId = "midilib"
            version = "2.0"

            afterEvaluate {
                from(components["release"])
            }
        }
    }
}

android {
    publishing {
        singleVariant("release") {
            withSourcesJar()
        }
    }

    namespace = "com.crylent.midilib"
    compileSdk = 34

    defaultConfig {
        minSdk = 24

        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"
        consumerProguardFiles("consumer-rules.pro")
        externalNativeBuild {
            cmake {
                cppFlags("-std=c++20 -fexceptions")
            }
        }
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
        }
        debug {
            isJniDebuggable = true
        }
    }
    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_17
        targetCompatibility = JavaVersion.VERSION_17
    }
    kotlinOptions {
        jvmTarget = "17"
    }
    externalNativeBuild {
        cmake {
            path = file("src/main/cpp/CMakeLists.txt")
            version = "3.22.1"
        }
    }
    ndkVersion = "25.2.9519653"
}

dependencies {
    implementation(libs.androidx.core.ktx)
    api(libs.oboe)
}

