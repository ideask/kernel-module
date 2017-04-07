#include<linux/module.h> 
#include<linux/init.h>  
extern void module_a_add_list(char *string, void(*module_fun)(void));  
extern void module_a_remove_list(char *string, void(*module_fun)(void));  
char *module_b_string="Bluetooth";
void module_b_fun(void)
{
//   printk("module_b:found this string is :%s \n Now is running the module_b_funtion\n",module_b_string);
    printk("Now is running module_b_function\n");
}
          
static int __init use_symbol_init(void)  
{    
   module_a_add_list(module_b_string,module_b_fun);
//   printk("module_b: add node \"%s\" successed!\n",module_b_string); 
   return 0;  
}  
static void __exit use_symbol_exit(void)  
{
   module_a_remove_list(module_b_string,module_b_fun);
//   printk("module_b: remove node \"%s \" successed!\n",module_b_string);

}  
      
module_init(use_symbol_init);  
module_exit(use_symbol_exit);  
MODULE_LICENSE("GPL");  
