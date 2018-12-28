# To avoid undefined reference to `__rust_probestack`.
# From https://github.com/rust-lang-nursery/compiler-builtins/blob/master/src/probestack.rs

	.global __rust_probestack
__rust_probestack:
	mov %rax, %r11
	cmp $0x1000, %r11
	jna 3f
2:
	sub $0x1000, %rsp
	test %rsp, 8(%rsp)
	sub $0x1000, %r11
	cmp $0x1000, %r11
	ja 2b
3:
	sub %r11, %rsp
	test %rsp, 8(%rsp)
	add %rax, %rsp
	ret
