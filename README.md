# Compiler

About:
This is a compiler that takes in a file or input of language .lan. The compiler
parses the data, builds a tree, and compilers the .lan language to .asm language built
specifically for the virtual machine included in the directory.

Built according to FS 2014 UMSL CS4280 course requirements.


Invocation:
comp [file]


Configuration:
Local static semantics
Local storage allocation


Known issues:
No known issues


Grammar:
<program> -> <var> do <block>  return
<block>   -> start <var> <stats> finish
<var>     -> empty | <type> ID <mvars> .
<type>    -> var
<mvars>   -> empty | : ID <mvars>
<expr>    -> <T> * <expr> | <T> / <expr> | <T>
<T>       -> <F> + <T> | <F> - <T> | <F>
<F>       -> - <F> | <R>
<R>       -> (<expr>) | ID | Number   
<stats>   -> <stat>  <mStat>
<mStat>   -> empty | <stat>  <mStat>
<stat>    -> <in> | <out> | <block> | <if> | <loop> | <assign>
<in>      -> read ID .
<out>     -> print <expr>  .
<if>      -> if [ <expr> <RO> <expr> ]  <block>             
<loop>    -> repeat [ <expr> <RO> <expr> ] <block>
<assign>  -> ID = <expr> .
<RO>      -> =< | => | == |  > | <  |  =!=