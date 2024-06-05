

select:     file format elf64-x86-64


Disassembly of section .init:

0000000000001000 <_init>:
    1000:	48 83 ec 08          	sub    $0x8,%rsp
    1004:	48 8b 05 c5 2f 00 00 	mov    0x2fc5(%rip),%rax        # 3fd0 <__gmon_start__@Base>
    100b:	48 85 c0             	test   %rax,%rax
    100e:	74 02                	je     1012 <_init+0x12>
    1010:	ff d0                	call   *%rax
    1012:	48 83 c4 08          	add    $0x8,%rsp
    1016:	c3                   	ret

Disassembly of section .plt:

0000000000001020 <printf@plt-0x10>:
    1020:	ff 35 ca 2f 00 00    	push   0x2fca(%rip)        # 3ff0 <_GLOBAL_OFFSET_TABLE_+0x8>
    1026:	ff 25 cc 2f 00 00    	jmp    *0x2fcc(%rip)        # 3ff8 <_GLOBAL_OFFSET_TABLE_+0x10>
    102c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000001030 <printf@plt>:
    1030:	ff 25 ca 2f 00 00    	jmp    *0x2fca(%rip)        # 4000 <printf@GLIBC_2.2.5>
    1036:	68 00 00 00 00       	push   $0x0
    103b:	e9 e0 ff ff ff       	jmp    1020 <_init+0x20>

Disassembly of section .plt.got:

0000000000001040 <__cxa_finalize@plt>:
    1040:	ff 25 9a 2f 00 00    	jmp    *0x2f9a(%rip)        # 3fe0 <__cxa_finalize@GLIBC_2.2.5>
    1046:	66 90                	xchg   %ax,%ax

Disassembly of section .text:

0000000000001050 <_start>:
    1050:	31 ed                	xor    %ebp,%ebp
    1052:	49 89 d1             	mov    %rdx,%r9
    1055:	5e                   	pop    %rsi
    1056:	48 89 e2             	mov    %rsp,%rdx
    1059:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
    105d:	50                   	push   %rax
    105e:	54                   	push   %rsp
    105f:	45 31 c0             	xor    %r8d,%r8d
    1062:	31 c9                	xor    %ecx,%ecx
    1064:	48 8d 3d ce 00 00 00 	lea    0xce(%rip),%rdi        # 1139 <main>
    106b:	ff 15 4f 2f 00 00    	call   *0x2f4f(%rip)        # 3fc0 <__libc_start_main@GLIBC_2.34>
    1071:	f4                   	hlt
    1072:	66 2e 0f 1f 84 00 00 	cs nopw 0x0(%rax,%rax,1)
    1079:	00 00 00 
    107c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000001080 <deregister_tm_clones>:
    1080:	48 8d 3d 91 2f 00 00 	lea    0x2f91(%rip),%rdi        # 4018 <__TMC_END__>
    1087:	48 8d 05 8a 2f 00 00 	lea    0x2f8a(%rip),%rax        # 4018 <__TMC_END__>
    108e:	48 39 f8             	cmp    %rdi,%rax
    1091:	74 15                	je     10a8 <deregister_tm_clones+0x28>
    1093:	48 8b 05 2e 2f 00 00 	mov    0x2f2e(%rip),%rax        # 3fc8 <_ITM_deregisterTMCloneTable@Base>
    109a:	48 85 c0             	test   %rax,%rax
    109d:	74 09                	je     10a8 <deregister_tm_clones+0x28>
    109f:	ff e0                	jmp    *%rax
    10a1:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
    10a8:	c3                   	ret
    10a9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

00000000000010b0 <register_tm_clones>:
    10b0:	48 8d 3d 61 2f 00 00 	lea    0x2f61(%rip),%rdi        # 4018 <__TMC_END__>
    10b7:	48 8d 35 5a 2f 00 00 	lea    0x2f5a(%rip),%rsi        # 4018 <__TMC_END__>
    10be:	48 29 fe             	sub    %rdi,%rsi
    10c1:	48 89 f0             	mov    %rsi,%rax
    10c4:	48 c1 ee 3f          	shr    $0x3f,%rsi
    10c8:	48 c1 f8 03          	sar    $0x3,%rax
    10cc:	48 01 c6             	add    %rax,%rsi
    10cf:	48 d1 fe             	sar    %rsi
    10d2:	74 14                	je     10e8 <register_tm_clones+0x38>
    10d4:	48 8b 05 fd 2e 00 00 	mov    0x2efd(%rip),%rax        # 3fd8 <_ITM_registerTMCloneTable@Base>
    10db:	48 85 c0             	test   %rax,%rax
    10de:	74 08                	je     10e8 <register_tm_clones+0x38>
    10e0:	ff e0                	jmp    *%rax
    10e2:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
    10e8:	c3                   	ret
    10e9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

00000000000010f0 <__do_global_dtors_aux>:
    10f0:	f3 0f 1e fa          	endbr64
    10f4:	80 3d 1d 2f 00 00 00 	cmpb   $0x0,0x2f1d(%rip)        # 4018 <__TMC_END__>
    10fb:	75 2b                	jne    1128 <__do_global_dtors_aux+0x38>
    10fd:	55                   	push   %rbp
    10fe:	48 83 3d da 2e 00 00 	cmpq   $0x0,0x2eda(%rip)        # 3fe0 <__cxa_finalize@GLIBC_2.2.5>
    1105:	00 
    1106:	48 89 e5             	mov    %rsp,%rbp
    1109:	74 0c                	je     1117 <__do_global_dtors_aux+0x27>
    110b:	48 8b 3d fe 2e 00 00 	mov    0x2efe(%rip),%rdi        # 4010 <__dso_handle>
    1112:	e8 29 ff ff ff       	call   1040 <__cxa_finalize@plt>
    1117:	e8 64 ff ff ff       	call   1080 <deregister_tm_clones>
    111c:	c6 05 f5 2e 00 00 01 	movb   $0x1,0x2ef5(%rip)        # 4018 <__TMC_END__>
    1123:	5d                   	pop    %rbp
    1124:	c3                   	ret
    1125:	0f 1f 00             	nopl   (%rax)
    1128:	c3                   	ret
    1129:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000001130 <frame_dummy>:
    1130:	f3 0f 1e fa          	endbr64
    1134:	e9 77 ff ff ff       	jmp    10b0 <register_tm_clones>

0000000000001139 <main>:
    1139:	55                   	push   %rbp
    113a:	48 89 e5             	mov    %rsp,%rbp
    113d:	48 83 ec 10          	sub    $0x10,%rsp
    1141:	f3 0f 10 05 f7 0e 00 	movss  0xef7(%rip),%xmm0        # 2040 <_IO_stdin_used+0x40>
    1148:	00 
    1149:	f3 0f 11 45 f4       	movss  %xmm0,-0xc(%rbp)
    114e:	66 0f ef c0          	pxor   %xmm0,%xmm0
    1152:	f3 0f 11 45 f8       	movss  %xmm0,-0x8(%rbp)
    1157:	66 0f ef c0          	pxor   %xmm0,%xmm0
    115b:	f3 0f 11 45 fc       	movss  %xmm0,-0x4(%rbp)
    1160:	66 0f ef c9          	pxor   %xmm1,%xmm1
    1164:	f3 0f 5a 4d f4       	cvtss2sd -0xc(%rbp),%xmm1
    1169:	f3 0f 10 05 d3 0e 00 	movss  0xed3(%rip),%xmm0        # 2044 <_IO_stdin_used+0x44>
    1170:	00 
    1171:	0f 2f 45 f4          	comiss -0xc(%rbp),%xmm0
    1175:	76 0a                	jbe    1181 <main+0x48>
    1177:	f2 0f 10 05 c9 0e 00 	movsd  0xec9(%rip),%xmm0        # 2048 <_IO_stdin_used+0x48>
    117e:	00 
    117f:	eb 04                	jmp    1185 <main+0x4c>
    1181:	66 0f ef c0          	pxor   %xmm0,%xmm0
    1185:	f2 0f 10 15 c3 0e 00 	movsd  0xec3(%rip),%xmm2        # 2050 <_IO_stdin_used+0x50>
    118c:	00 
    118d:	f2 0f 59 d0          	mulsd  %xmm0,%xmm2
    1191:	f3 0f 10 45 f4       	movss  -0xc(%rbp),%xmm0
    1196:	0f 2f 05 bb 0e 00 00 	comiss 0xebb(%rip),%xmm0        # 2058 <_IO_stdin_used+0x58>
    119d:	76 15                	jbe    11b4 <main+0x7b>
    119f:	f3 0f 10 05 b5 0e 00 	movss  0xeb5(%rip),%xmm0        # 205c <_IO_stdin_used+0x5c>
    11a6:	00 
    11a7:	0f 2f 45 f4          	comiss -0xc(%rbp),%xmm0
    11ab:	76 07                	jbe    11b4 <main+0x7b>
    11ad:	b8 01 00 00 00       	mov    $0x1,%eax
    11b2:	eb 05                	jmp    11b9 <main+0x80>
    11b4:	b8 00 00 00 00       	mov    $0x0,%eax
    11b9:	66 0f ef db          	pxor   %xmm3,%xmm3
    11bd:	f2 0f 2a d8          	cvtsi2sd %eax,%xmm3
    11c1:	f2 0f 10 05 97 0e 00 	movsd  0xe97(%rip),%xmm0        # 2060 <_IO_stdin_used+0x60>
    11c8:	00 
    11c9:	f2 0f 59 c3          	mulsd  %xmm3,%xmm0
    11cd:	f2 0f 58 c2          	addsd  %xmm2,%xmm0
    11d1:	f2 0f 59 c1          	mulsd  %xmm1,%xmm0
    11d5:	f2 0f 5a c0          	cvtsd2ss %xmm0,%xmm0
    11d9:	f3 0f 11 45 fc       	movss  %xmm0,-0x4(%rbp)
    11de:	66 0f ef c9          	pxor   %xmm1,%xmm1
    11e2:	f3 0f 5a 4d f4       	cvtss2sd -0xc(%rbp),%xmm1
    11e7:	f3 0f 10 45 f4       	movss  -0xc(%rbp),%xmm0
    11ec:	0f 2f 05 65 0e 00 00 	comiss 0xe65(%rip),%xmm0        # 2058 <_IO_stdin_used+0x58>
    11f3:	76 15                	jbe    120a <main+0xd1>
    11f5:	f3 0f 10 05 5f 0e 00 	movss  0xe5f(%rip),%xmm0        # 205c <_IO_stdin_used+0x5c>
    11fc:	00 
    11fd:	0f 2f 45 f4          	comiss -0xc(%rbp),%xmm0
    1201:	76 07                	jbe    120a <main+0xd1>
    1203:	b8 01 00 00 00       	mov    $0x1,%eax
    1208:	eb 05                	jmp    120f <main+0xd6>
    120a:	b8 00 00 00 00       	mov    $0x0,%eax
    120f:	66 0f ef d2          	pxor   %xmm2,%xmm2
    1213:	f2 0f 2a d0          	cvtsi2sd %eax,%xmm2
    1217:	f2 0f 10 05 31 0e 00 	movsd  0xe31(%rip),%xmm0        # 2050 <_IO_stdin_used+0x50>
    121e:	00 
    121f:	f2 0f 59 d0          	mulsd  %xmm0,%xmm2
    1223:	f3 0f 10 45 f4       	movss  -0xc(%rbp),%xmm0
    1228:	0f 2f 05 39 0e 00 00 	comiss 0xe39(%rip),%xmm0        # 2068 <_IO_stdin_used+0x68>
    122f:	76 0a                	jbe    123b <main+0x102>
    1231:	f2 0f 10 05 0f 0e 00 	movsd  0xe0f(%rip),%xmm0        # 2048 <_IO_stdin_used+0x48>
    1238:	00 
    1239:	eb 04                	jmp    123f <main+0x106>
    123b:	66 0f ef c0          	pxor   %xmm0,%xmm0
    123f:	f2 0f 10 1d 29 0e 00 	movsd  0xe29(%rip),%xmm3        # 2070 <_IO_stdin_used+0x70>
    1246:	00 
    1247:	f2 0f 59 c3          	mulsd  %xmm3,%xmm0
    124b:	f2 0f 58 c2          	addsd  %xmm2,%xmm0
    124f:	f2 0f 59 c1          	mulsd  %xmm1,%xmm0
    1253:	f2 0f 5a c0          	cvtsd2ss %xmm0,%xmm0
    1257:	f3 0f 11 45 f8       	movss  %xmm0,-0x8(%rbp)
    125c:	66 0f ef e4          	pxor   %xmm4,%xmm4
    1260:	f3 0f 5a 65 f4       	cvtss2sd -0xc(%rbp),%xmm4
    1265:	66 48 0f 7e e0       	movq   %xmm4,%rax
    126a:	66 48 0f 6e c0       	movq   %rax,%xmm0
    126f:	48 8d 05 92 0d 00 00 	lea    0xd92(%rip),%rax        # 2008 <_IO_stdin_used+0x8>
    1276:	48 89 c7             	mov    %rax,%rdi
    1279:	b8 01 00 00 00       	mov    $0x1,%eax
    127e:	e8 ad fd ff ff       	call   1030 <printf@plt>
    1283:	66 0f ef ed          	pxor   %xmm5,%xmm5
    1287:	f3 0f 5a 6d fc       	cvtss2sd -0x4(%rbp),%xmm5
    128c:	66 48 0f 7e e8       	movq   %xmm5,%rax
    1291:	66 48 0f 6e c0       	movq   %rax,%xmm0
    1296:	48 8d 05 7b 0d 00 00 	lea    0xd7b(%rip),%rax        # 2018 <_IO_stdin_used+0x18>
    129d:	48 89 c7             	mov    %rax,%rdi
    12a0:	b8 01 00 00 00       	mov    $0x1,%eax
    12a5:	e8 86 fd ff ff       	call   1030 <printf@plt>
    12aa:	66 0f ef f6          	pxor   %xmm6,%xmm6
    12ae:	f3 0f 5a 75 f8       	cvtss2sd -0x8(%rbp),%xmm6
    12b3:	66 48 0f 7e f0       	movq   %xmm6,%rax
    12b8:	66 48 0f 6e c0       	movq   %rax,%xmm0
    12bd:	48 8d 05 5f 0d 00 00 	lea    0xd5f(%rip),%rax        # 2023 <_IO_stdin_used+0x23>
    12c4:	48 89 c7             	mov    %rax,%rdi
    12c7:	b8 01 00 00 00       	mov    $0x1,%eax
    12cc:	e8 5f fd ff ff       	call   1030 <printf@plt>
    12d1:	f3 0f 10 45 f4       	movss  -0xc(%rbp),%xmm0
    12d6:	f3 0f 58 45 fc       	addss  -0x4(%rbp),%xmm0
    12db:	f3 0f 5c 45 f8       	subss  -0x8(%rbp),%xmm0
    12e0:	66 0f ef ff          	pxor   %xmm7,%xmm7
    12e4:	f3 0f 5a f8          	cvtss2sd %xmm0,%xmm7
    12e8:	66 48 0f 7e f8       	movq   %xmm7,%rax
    12ed:	66 48 0f 6e c0       	movq   %rax,%xmm0
    12f2:	48 8d 05 3a 0d 00 00 	lea    0xd3a(%rip),%rax        # 2033 <_IO_stdin_used+0x33>
    12f9:	48 89 c7             	mov    %rax,%rdi
    12fc:	b8 01 00 00 00       	mov    $0x1,%eax
    1301:	e8 2a fd ff ff       	call   1030 <printf@plt>
    1306:	b8 00 00 00 00       	mov    $0x0,%eax
    130b:	c9                   	leave
    130c:	c3                   	ret

Disassembly of section .fini:

0000000000001310 <_fini>:
    1310:	48 83 ec 08          	sub    $0x8,%rsp
    1314:	48 83 c4 08          	add    $0x8,%rsp
    1318:	c3                   	ret
