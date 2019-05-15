.class public data_type
.super java/lang/Object

.field private static _runTimer LRunTimer;
.field private static _standardIn LPascalTextIn;
.field private static a I
.field private static b Ljava/lang/String;

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
	putstatic        data_type/_runTimer LRunTimer;
	new PascalTextIn
	dup
	invokenonvirtual PascalTextIn/<init>()V
	putstatic        data_type/_standardIn LPascalTextIn;
	ldc	12
	putstatic	data_type/a I
	ldc	"cat"
	putstatic	data_type/b Ljava/lang/String;
	getstatic java/lang/System/out Ljava/io/PrintStream;
	ldc	"Variable a is: %d"
	ldc	1
	anewarray java/lang/Object
	dup
	ldc	0
	getstatic	data_type/a I
	invokestatic java/lang/Integer.valueOf(I)Ljava/lang/Integer;
	aastore
	invokestatic  java/lang/String.format(Ljava/lang/String;[Ljava/lang/Object;)Ljava/lang/String;
	invokevirtual java/io/PrintStream/println(Ljava/lang/String;)V
	getstatic java/lang/System/out Ljava/io/PrintStream;
	ldc	"String b is: %s"
	ldc	1
	anewarray java/lang/Object
	dup
	ldc	0
	getstatic	data_type/b Ljava/lang/String;
	aastore
	invokestatic  java/lang/String.format(Ljava/lang/String;[Ljava/lang/Object;)Ljava/lang/String;
	invokevirtual java/io/PrintStream/println(Ljava/lang/String;)V

	getstatic     data_type/_runTimer LRunTimer;
	invokevirtual RunTimer.printElapsedTime()V

	return

.limit locals 16
.limit stack 16
.end method
