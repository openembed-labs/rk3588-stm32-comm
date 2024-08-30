

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
