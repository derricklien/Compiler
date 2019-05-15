.class public function
.super java/lang/Object

.field private static _runTimer LRunTimer;
.field private static _standardIn LPascalTextIn;
.field private static sum_num I
.field private static sum_count I
.field private static sum_ans I
.field private static sum_i I

.method public <init>()V

	aload_0
	invokenonvirtual    java/lang/Object/<init>()V
	return

.limit locals 1
.limit stack 1
.end method

.method public static main([Ljava/lang/String;)V

	new RunTimer
	dup
	invokenonvirtual RunTimer/<init>()V
	putstatic        function/_runTimer LRunTimer;
	new PascalTextIn
	dup
	invokenonvirtual PascalTextIn/<init>()V
	putstatic        function/_standardIn LPascalTextIn;
	goto sum_end
sum:
	astore_1
	ldc	0
	putstatic	function/sum_ans I
	ldc	0
	putstatic	function/sum_i I
Label_0:
	getstatic	function/sum_i I
	getstatic	function/sum_count I
	if_icmplt Label_1
	goto Label_2
Label_1:
	getstatic	function/sum_ans I
	getstatic	function/sum_num I
	iadd
	putstatic	function/sum_ans I
	getstatic	function/sum_i I
	iconst_1
	iadd
	putstatic	function/sum_i I
	goto Label_0
Label_2:
	getstatic	function/sum_ans I
	ret 1
sum_end:
	getstatic java/lang/System/out Ljava/io/PrintStream;
	ldc	"The sum is: %d"
	ldc	1
	anewarray java/lang/Object
	dup
	ldc	0
	ldc	1
	putstatic	function/sum_num I
	ldc	2
	putstatic	function/sum_count I
	jsr sum
	invokestatic java/lang/Integer.valueOf(I)Ljava/lang/Integer;
	aastore
	invokestatic  java/lang/String.format(Ljava/lang/String;[Ljava/lang/Object;)Ljava/lang/String;
	invokevirtual java/io/PrintStream/println(Ljava/lang/String;)V

	getstatic     function/_runTimer LRunTimer;
	invokevirtual RunTimer.printElapsedTime()V

	return

.limit locals 16
.limit stack 16
.end method
