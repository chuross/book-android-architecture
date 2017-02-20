= 開発時とリリース時で設定をビルドで切り替える

== 概要
Androidを開発している時に開発版と本番で細かくビルド設定を変更したい場合がある。

例えばこんなケース

 * 開発向きの場合はデバッグフラグとは別に開発環境向きフラグを立てたい
 * デバッグ時だけMultiDexを有効にしたい
 * リリース時だけProguardをかけたい
 * 見分けやすいように開発と本番でアプリ名を分けたい

 他にも理由は沢山挙げられるが、何かと変更したい部分である。

 こうした場合には@<b>{Build Variants}を活用すると便利である。

== Build Variantsとは何か
Gradle Android Pluginが提供している仕組み。

この仕組みを使うと、@<b>{Build Types}と@<b>{Product Flavors}という要素を組み合わせて色々なビルドを実現することができる。

以下は必ずしもこの粒度で切る必要は無いが、使い分けの例としてはこのような感じ

: Build Types
   debug・releaseといった粒度

: Product Flavors
   develop(開発環境)・production(本番環境)といった粒度

===[column] 上記の例で作れる組み合わせ
: developDebug
   開発環境向きデバッグ
: productionDebug
   本番環境向きデバッグ
: productionRelease
   リリースビルド

本番向きデバッグとの違いはProguardの有無やGoogleAnalytics向き先など
===[/column]

== build.gradleにBuild Variantsを追加する
簡単な例として開発環境と本番環境をbuild.gradleで振り分けるようにする。

追加するための記述自体は簡単で、Androidプロジェクトルートにあるbuild.gradleに@<b>{productFlavors}を追加するだけである。

//source[Product FlavorsでアプリケーションIDを切り替えた例]{
android {
  ....
  productFlavors {
    develop {
      applicationId: 'com.github.chuross.example.dev'
    }

    production {
      applicationId: 'com.github.chuross.example'
    }
  }
}
//}

この記述によってGradleのタスクが自動生成される。

以下のようなコマンドを実行することで開発と本番でアプリケーションIDを切り替えたビルドが可能になる。

//cmd{
// 開発環境向きデバッグ
$ ./gradlew assembleDevelopDebug

// 本番環境向きデバッグ
$ ./gradlew assembleProductionDebug
//}

アプリケーションIDが切り替えできると、端末内に開発向きと本番向きで別々のアプリとして同じ端末にインストールできるようになる。

ちなみに上記にはBuildTypeを明記していないが、@<b>{debug}と@<b>{release}のBuild TypeはAndroid Gradle Pluginがデフォルトで用意されている。

== 応用編 - メモリリーク確認用ビルドを作る
LeakCanaly@<bib>{leak-canary}を用いてメモリリークチェック用のビルドをGradleで実現する。

この方法を使うと、開発環境・本番環境ビルドでメモリリークしてないか事前に調べることができるので便利。

実際にはJavaのコードにも手を加える必要があるが、ここではbuild.gradleだけフォーカスして説明する。

//source[LeakCanaryを導入したbuild.gradle]{
android {
  productFlavors {
    develop { .... }
    production { .... }
  }
  ....
  buildTypes {
    debug { .... }
    release { .... }
    leakCheck { .... }
  }
  ...
  dependencies {
    leakCheckCompile 'com.squareup.leakcanary:leakcanary-android:1.5'
    debugCompile 'com.squareup.leakcanary:leakcanary-android-no-op:1.5'
    releaseCompile 'com.squareup.leakcanary:leakcanary-android-no-op:1.5'
  }
}
//}

上記の形でbuild.gradleを記述すると以下のような形でリークチェック用のビルドが実行できる。

//cmd{
$ ./gradlew assembleDevelopLeakCheck
$ ./gradlew assembleProductionLeakCheck
//}
