// SPDX-License-Identifier: GPL-2.0 OR BSD-3-Clause
/*
 * Copyright (c) 2017, GlobalLogic Ukraine LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the GlobalLogic.
 * 4. Neither the name of the GlobalLogic nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GLOBALLOGIC UKRAINE LLC ``AS IS`` AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC UKRAINE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/delay.h>

#include <linux/ktime.h>
#include <linux/list.h>

struct time_list {
	struct list_head list;
	ktime_t time;
};


MODULE_AUTHOR("Jack Rybnikov IO-81");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static LIST_HEAD(head);

static uint times = 1;
module_param(times, uint, 0444); // S_IRUGO
MODULE_PARM_DESC(times, "The amount of times to print hello world");

static int __init initter(void)
{
	uint i = 0;
	struct time_list *temp, *next;
	BUG_ON(times > 10);

	if (times == 0)
		pr_warn("The parameter is %d\n", times);
	else if (times >= 5 && times <= 10)
		pr_warn("The parameter is %d, between 5 and 10\n", times);

	for (i = 0; i < times; i++) {
		temp = kmalloc(sizeof(struct time_list), GFP_KERNEL);
		if (i == 7) // for debug purposes
			temp = NULL;
		//if (ZERO_OR_NULL_PTR(temp))
			//goto error;

		temp->time = ktime_get();
		pr_info("Hello World!\n");
		list_add_tail(&temp->list, &head);
	}

	return 0;

error:

	pr_err("Out of memory\n");
	list_for_each_entry_safe(temp, next, &head, list) {
		list_del(&temp->list);
		kfree(temp);
	}

	if (!list_empty(&head))
		return -ENOMEM;

	return 0;
}

static void __exit exitter(void)
{
	struct time_list *temp, *next;
	list_for_each_entry_safe(temp, next, &head, list) {
		pr_info("Time spent to print: %lld\n", temp->time);
		list_del(&temp->list);
		kfree(temp);
	}
}


module_init(initter);
module_exit(exitter);
