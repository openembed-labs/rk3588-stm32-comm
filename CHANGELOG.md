

## [0.5.0](https://github.com/openembed-labs/rk3588-stm32-comm/compare/0.4.0...0.5.0) (2024-11-15)


### ✨ Features | 新功能

* update dec ([3c8666a](https://github.com/openembed-labs/rk3588-stm32-comm/commit/3c8666a6e143e869290bdc41a9951f41d95cd44b))
* 优化测试 ([3ef22b4](https://github.com/openembed-labs/rk3588-stm32-comm/commit/3ef22b443e53bc7bc7d4e659325918c799a4590e))

## [0.4.0](https://github.com/openembed-labs/rk3588-stm32-comm/compare/0.3.0...0.4.0) (2024-09-29)


### ✨ Features | 新功能

* docs & 单独线程检测文件IO ([4713bb5](https://github.com/openembed-labs/rk3588-stm32-comm/commit/4713bb5b7a0d8fb3fd7e78fbe88b4a587bcc0b14))
* DO高低电平交替输出 ([fe88d9d](https://github.com/openembed-labs/rk3588-stm32-comm/commit/fe88d9dde7adc3da0c57c04af30e62d17ce87abb))
* upgrade firmware STM32H750固件升级 & 新增升级指令、帮助指令 ([809ddbe](https://github.com/openembed-labs/rk3588-stm32-comm/commit/809ddbe3ee7254fb70370cf9628463e442601b53))
* 两个程序之间使用信号量通知命令行输入扫描 ([98268ff](https://github.com/openembed-labs/rk3588-stm32-comm/commit/98268ff52cc070a26fca9cc09bf68501cefe644d))
* 代码优化 & 收发拦截器 ([abd8466](https://github.com/openembed-labs/rk3588-stm32-comm/commit/abd84665630d0f74ea964de7c5325b289d964a5c))
* 增加交互命令行模式 & 客户端改为多线程 ([3c2530d](https://github.com/openembed-labs/rk3588-stm32-comm/commit/3c2530d0377e8e710fc4f15588bc668b28c06ec5))
* 增加命令行发送 代码结构调整 ([b3be851](https://github.com/openembed-labs/rk3588-stm32-comm/commit/b3be85188767824d8142e13b23ede27d97733d40))
* 将同时多个DO口发送改完单个 ([fb9a23e](https://github.com/openembed-labs/rk3588-stm32-comm/commit/fb9a23e9346cdf9790c0b4493144bbc22fb25573))
* 心跳检测复位 & GPIO控制 ([7bc53ff](https://github.com/openembed-labs/rk3588-stm32-comm/commit/7bc53ffceeb6458bb6f1ddcd1f6885f3895bf1f3))
* 改变重启客户端机制 ([a1ba3e6](https://github.com/openembed-labs/rk3588-stm32-comm/commit/a1ba3e6fc4ae7c1199193fca7c7a6570a3e68218))
* 新增ndev测试模式 ([43ec1b0](https://github.com/openembed-labs/rk3588-stm32-comm/commit/43ec1b007b64f6e6e1ffd68574b6993e2984082f))
* 进行升级时 暂停其他传输行为 ([0c3cbb5](https://github.com/openembed-labs/rk3588-stm32-comm/commit/0c3cbb5609b672a11998fa7ef3387287ab031535))


### 📝 Documentation | 文档

* updated ([95f9352](https://github.com/openembed-labs/rk3588-stm32-comm/commit/95f9352fe37985bcc9f16c25b4d4c0776194fd60))
* 测试相关文档 ([0de6466](https://github.com/openembed-labs/rk3588-stm32-comm/commit/0de64661897e4d55b330d0b3127f1e324cf9fea2))

## [0.3.0](https://github.com/openembed-labs/rk3588-stm32-comm/compare/0.2.0...0.3.0) (2024-08-30)


### ✨ Features | 新功能

* decode DI data ([483efed](https://github.com/openembed-labs/rk3588-stm32-comm/commit/483efed35e449505ebc37c83fa4f709772b81c00))
* send_device_data 构造发送数据 ([5bd3157](https://github.com/openembed-labs/rk3588-stm32-comm/commit/5bd31579c4275970b12a7298fd54a0b15927aa4f))
* server文件拆分 ([16b7014](https://github.com/openembed-labs/rk3588-stm32-comm/commit/16b70147f1221a6a14236b772ef481fcbf65ed32))
* 创建多进程处理客户端连接 ([3800fc9](https://github.com/openembed-labs/rk3588-stm32-comm/commit/3800fc95b8ae87a271519363fefc5fd803661d3e))
* 去除send_ready限制 服务端不再等待客户端准备完成 & 增加收发测试MODE & 完善文档 ([f39ce6a](https://github.com/openembed-labs/rk3588-stm32-comm/commit/f39ce6ab059b161e2fa4e9284ba9444c5edc127c))
* 多线程收发数据 构造含设备号的数据 ([dc359db](https://github.com/openembed-labs/rk3588-stm32-comm/commit/dc359dba2788a2ae84680cd6d441533b971052d7))
* 完善客户端代码 ([96c1b8d](https://github.com/openembed-labs/rk3588-stm32-comm/commit/96c1b8d5c9087551ee453cf284e798a01a2286ca))
* 构建测试方法 ([0b77faa](https://github.com/openembed-labs/rk3588-stm32-comm/commit/0b77faa4a24ba829b3eddb605efae85f91bd7efe))


### 🐛 Bug Fixes | Bug 修复

* 修复数据接收延迟，打印错乱，服务端意外关闭 ([64d0e59](https://github.com/openembed-labs/rk3588-stm32-comm/commit/64d0e5996b60fb25ca2878df675b30abd330f98a))

## 0.2.0 (2024-08-26)


### ✨ Features | 新功能

* --daemon ([e6e18b6](https://github.com/openembed-labs/rk3588-stm32-comm/commit/e6e18b6852f10f342624a41f7cc446e90f5eae9b))
* daemonize ([eedecd7](https://github.com/openembed-labs/rk3588-stm32-comm/commit/eedecd7e860b2cfd0c95522491e5d6fba372d551))
* logger ([4486416](https://github.com/openembed-labs/rk3588-stm32-comm/commit/448641620b2b114cef23467ef84c1e8d9eb6485e))
* make cross_build ([d3b28d9](https://github.com/openembed-labs/rk3588-stm32-comm/commit/d3b28d96306b0bcdaf2b0a9a8f5d3feee223cf3e))
* opmit dir and code style ([90cd52a](https://github.com/openembed-labs/rk3588-stm32-comm/commit/90cd52a5bf2c02a5862116d271f54a0734bafec9))
* release ([aef277d](https://github.com/openembed-labs/rk3588-stm32-comm/commit/aef277dc9659d0d2929c79fd703e807433f108d6))
* select server or client ([e799b41](https://github.com/openembed-labs/rk3588-stm32-comm/commit/e799b41bf1cbd6e941d55693619123b226a35e2c))
* server response msg ([d35a0c0](https://github.com/openembed-labs/rk3588-stm32-comm/commit/d35a0c05aaa82532c80bf911782e1c598313ec7b))
* simple msg ([746cd73](https://github.com/openembed-labs/rk3588-stm32-comm/commit/746cd7308530ed7d0e0bb3ef30673d2779c89b9a))


### 📝 Documentation | 文档

* update doc ([ff5e0b0](https://github.com/openembed-labs/rk3588-stm32-comm/commit/ff5e0b04fb3d12281f04f490c0ae6699bb4fda64))
