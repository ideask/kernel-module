#include<linux/module.h> 
#include<linux/init.h>  
extern void module_a_add_list(char *string, void(*module_fun)(void));  
extern void module_a_remove_list(char *string, void(*module_fun)(void));  
char *module_c_string="Headphone";
void module_c_fun(void)
{
//   printk("module_c:found this string is :%s \n Now is running the module_c_funtion\n",module_c_string);
   printk("Now is running module_c_function\n");
}
          
static int __init use_symbol_init(void)  
{    
   module_a_add_list(module_c_string,module_c_fun);
//   printk("module_c: add node \"%s\" successed!\n",module_c_string); 
   return 0;  
}  
static void __exit use_symbol_exit(void)  
{
   module_a_remove_list(module_c_string,module_c_fun);
//   printk("module_c: remove node \"%s \" successed!\n",module_c_string);

}  
      
module_init(use_symbol_init);  
module_exit(use_symbol_exit);  
MODULE_LICENSE("GPL");  
