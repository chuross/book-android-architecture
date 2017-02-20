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

===[column] 上記の例によって作れる組み合わせ
@<b>{developDebug}: 開発環境向きデバッグ
@<b>{productionDebug}: 本番環境向きデバッグ
@<b>{productionRelease}: リリースビルド

本番向きデバッグとの違いはProguardの有無やGoogleAnalytics向き先など
===[/column]

== build.gradleにBuild Variantsを追加する
簡単な例として開発環境と本番環境をbuild.gradleで振り分けるようにする。

追加するための記述自体は簡単で、Androidプロジェクトルートにあるbuild.gradleに@<b>{productFlavors}を追加するだけである。

//emlist[Product FlavorsでアプリケーションIDを切り替えた例][gradle]{
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

//emlist[ビルドの切り替え][bash]{
// 開発環境向きデバッグ
$ ./gradlew assembleDevelopDebug

// 本番環境向きデバッグ
$ ./gradlew assembleProductionDebug
//}

アプリケーションIDが切り替えできると、端末内に開発向きと本番向きで別々のアプリとして同じ端末にインストールできるようになる。
