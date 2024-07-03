# Dark 帝国 - 数据中心

当你拿着令牌想要离开的时候, 意外发生了.

"wtf! 这个门怎么又锁起来." 原来, 无论是进入还是离开数据中心, 都需要通过终端机进行验证. 验证方法还是一样, 登录密码. "那就再来一次吧" 然而, 这次, 你在调用了 `ls` 了以后, 却发现原来的文件没有了. "woc, 难道是我刚才手贱删文件了, 还是说两边验证系统还不一样......" 慌忙中, 你赶紧向数据中心的 AI 询问在忘记密码后该如何补救.

"忘记密码后可以采取以下几种方法进行补救:"

"使用密码恢复功能: 大多数网站和服务都提供了密码恢复或重置功能. 通常可以通过点击登录页面上的 '忘记密码' 或类似链接, 输入注册邮箱或用户名, 然后按照系统提示操作, 通过电子邮件或短信获取重置密码的链接或验证码."

"回答安全问题: 一些服务在设置账户时会要求用户设置安全问题. 如果你记得答案, 可以使用这种方法重置密码."

"使用密码管理工具: 为了避免忘记密码，可以使用密码管理工具, 如WKPassword, JPPass等, 这些工具可能存储了密码的备份"

"联系客户支持: 如果以上方法都不奏效，可以联系网站或服务的客户支持团队，提供必要的身份验证信息，以便他们帮助你重置密码"

"......" 你沉默良久, 为了尽量不惊扰其他人, 你决定尝试本地的密码恢复工具: JPPass. 你查看了 JPPass 的源代码和本地存储. 然而, 非常搞笑的是, 你发现这个工具备份的是密码在验证系统中的哈希值, 而不是密码本身......