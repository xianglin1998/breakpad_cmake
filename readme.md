# 为什么要用CMAKE？
1. breakpad从2018年提交了一个commit之后，就没办法用gyp生成VS的sln项目进行编译输出lib了，现在只能用mingw进行./configure & make，生成的库只有libbreakpad.a
2. 折腾了两天，最初以为mingw输出的.a库和VS的cmake + msvc的工程依赖的.lib库是同一个东西，但是链接的时候始终是缺少某些资源，查资料且看编译过程，发现msvc和mingw使用的gcc其实是两种编译器了，跨编译器的库依赖非常复杂而且可能有隐藏问题，于是放弃研究.a转.lib，开始研究CMAKE编译breakpad

# 为什么一定要用CMAKE且编译出来.lib？
1. 已经成型的项目是cmake管理的，开发团队统一使用VS + CMAKE进行项目开发，没精力移植到mingw端再去写makefile
2. 跨平台的项目，在windows端已经有其他的依赖库是.lib的，比如pthread，不方便更换。

# 一定要用CMAKE吗？
1. 不一定，对于Linux平台开发来说，项目最终编译和运行的环境就是Linux，比如Debian或者Ubuntu，Linux平台测试和生产环境都可以考虑安装libbreakpad的prebuild库或者手动编译，libbreakpad安装好了之后，我们依赖breakpad的库就行了，就算是VS的remote debug都不影响使用。
2. 我们最终的目的就是编译breakpad，将其编译为static或者shared库，在Windows上进行开发需要msvc且依赖.lib的，可以考虑使用此cmake编译，而linux直接用./configure & make无脑编译就好了，mac端更是有自带xcode的工程，没必要cmake。浪费时间就是浪费生命。

# 还有一些注意事项
1. breakpad官方设计dump文件格式的时候，为了统一，决策后采用了Microsoft的minidump格式。
2. 根据官方文档描述breakpad的架构就是：
    - client，也就是libbreakpad.a(.lib)，运行在我们最终的.exe可执行程序上的，用于注册监听crash事件，发生crash事件后将当前堆栈信息进行记录为minidump文件。
    - dumper，也就是把可执行程序的debug信息提取出来成为breakpad规范的可识别symbol文件，这个symbol文件是后面定位问题能到某一行代码上的关键。
    - processor，在client崩溃后得到输出的 'guid.dmp' 堆栈信息文件，processor加载.dmp和dumper导出的symbol文件，就可以debug到出问题的代码行。包括汇编信息那些。
3. 上面描述的架构是linux和mac上用的，因此编译最好是用breakpad官方提供的编译脚本或者工程，他们会管理好所有的依赖，最终生成libbreakpad.a(client)，dump_syms.exe(dumper)，minidump_stackwalk.exe(processor)，但是windows上用vs+cmake开发，我们不需client之外的东西，因为VS原生支持.dmp文件的解析，双击.dmp文件打开VS面板，然后symbol文件选择你输出的exe编译时输出的.pdb，就能直接解析到问题点了。
4. c++?，还是算了吧，我现在维护的一个工程是98%血统的C工程，所以 `c_wapper` 存在的意义就是把breakpad的初始化注册的c++流程用c包装一遍，所以如果你遇到在C项目上使用breakpad的需求，可以参考一下 `c_wapper` 目录下的写法


# 最后再说一下，目前的仓库只提供编译Windows端的msvc库文件的能力，有其他的需要的直接忽略本仓库，去breakpad官方仓库阅读文档编译吧。
