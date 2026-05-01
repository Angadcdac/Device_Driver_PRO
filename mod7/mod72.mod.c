#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x3726c6aa, "module_layout" },
	{ 0x695bf5e9, "hrtimer_cancel" },
	{ 0xec523f88, "hrtimer_start_range_ns" },
	{ 0xa362bf8f, "hrtimer_init" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x5cc2a511, "hrtimer_forward" },
	{ 0xc5850110, "printk" },
};

MODULE_INFO(depends, "");

