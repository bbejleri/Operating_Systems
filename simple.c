# Sisteme_Operimi

/* Krijimi i nje moduli kernel dhe ngarkimi i tij ne kernelin e Linux.
 *Duke perdorur listat e lidhura ne strukture, Linux ben te mundur menaxhimin e te dhenave te struktures duke perdorur macro funksionet.
 BORA BEJLERI
 */
 
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/slab.h> /* Per te perdorur funksionet kmalloc dhe kfree.*/


/* Deklarimi i struktures ditelindje */
struct birthday
{
    int dita;
    int muaji;
    int viti;
    struct list_head list; /* koka e listes (lsit head) e deklaruar ne librarine <linux/types.h>. */
};

static LIST_HEAD( liste_birthday ); /* Objekt koke liste per tu perdorur si reference per koken e listes duke perdorur LIST HEAD() macro */

/*Funksioni qe do te do te therritet gjate ngarkimit te modulit ne kernel. Shton 6 nyje ne liste. Bredh listen dhe afishon vlerat e nyjeve ne terminal.*/

int simple_init( void ) {
        printk( KERN_INFO "\t\tDuke testuar krijimin e modulit.\n" );



        struct birthday *bora;

        int i;

        for( i = 1; i <= 5; i++ ) /* 5 here (nyje) */
        {
            bora = kmalloc( sizeof(*bora), GFP_KERNEL ); /* kmalloc() i njeje me user-level malloc() per te alokuar memorje */
            bora->dita = i;
            bora->muaji = i + 7;
            bora->viti = 1995;
            INIT_LIST_HEAD(&bora->list );

            list_add_tail( &bora->list, &liste_birthday ); /*fundi i listes*/
        }


        /* bredhja */
        struct birthday *ptr;

        int v = 0;

        list_for_each_entry( ptr, &liste_birthday, list ) /*Levizja ne liste. 3 parametra, 1 shenon ne strukturen qe perseritet, 2 pointer ne koke, 3 emri i variablit te kokes se listes*/ 
        {
            v++;

            printk( KERN_INFO "Bora %d: born  %d-%d-%d \n", v, ptr->dita, ptr->muaji, ptr->viti );
        }
    return 0;
} /* fundi i funksionit simple_init*/


/**
  *  Funksioni qe therritet per fshirjen e modulit nga kerneli. Bredh listen dhe fshin cdo nyje nga lista */
 
void simple_exit( void )
{
    printk( KERN_INFO "\t\tDuke testuar fshirjen e modulit.\n" );

    struct birthday *ptr, *pas;

    int v = 0;

    list_for_each_entry_safe(ptr, pas, &liste_birthday, list )
    {
        v++;

        printk( KERN_INFO "Deleting Bora %d\n", v );

        list_del(&ptr->list);

        kfree(ptr);
    }


} /* Fund funksioni simple_exit */


module_init( simple_init ); /*entry point */
module_exit( simple_exit ); /* exit point */

MODULE_LICENSE( "GPL" );
MODULE_DESCRIPTION( "Modul i thjeshte" );
MODULE_AUTHOR( "BORA BEJLERI" );
