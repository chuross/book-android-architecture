= Build Variantsを活用して開発時とリリース時で設定を使い分ける

== 概要
Androidを開発している時に開発版と本番で細かくビルド設定を変更したい場合がある。

例えばこんなケース

 * 開発版だけデバッグフラグを立てたい
 * 開発版だけMultiDexを有効にしたい
 * 本番の時はProguardを実行したい
 * 見分けやすいように開発と本番でアプリ名を分けたい

 他にも理由は沢山挙げられるが、何かと変更したい部分である。

===[column] Build Variantsとは何か
Build TypesとProduct Flavorsで構成するビルドの組み合わせ

以下は必ずしもこの粒度で切る必要は無いが、使い分けの例としてはこのような感じ

==== Build Types
debug・releaseといった粒度

==== Product Flavors
develop(開発環境)・production(本番環境)といった粒度

===[/column]

== build.gradleにBuild Variantsを追加する
これからかく
