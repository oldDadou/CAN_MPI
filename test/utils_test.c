#include <stdio.h>
#include "CUnit/Basic.h"

#include "../src/can_communication.h"
#include "../src/utils.h"
#include "../src/cartesian_space.h"

#define UNUSED(x) (void)(x)

int init_suite1 (void) {return 0;}

int clean_suite1(void) {return 0;}

void test_42(void) {
  unsigned int life = 42;
  CU_ASSERT(42 == life);
}

void test_INIT_LIST(void) {
  list nghbrs;
  init_list(&nghbrs, sizeof(neighbour));
  CU_ASSERT(nghbrs.nb_elem == 0);
  CU_ASSERT(nghbrs.first == NULL);
  CU_ASSERT(nghbrs.element_size == sizeof(neighbour));
}

void test_LIST_ADD_FRONT(void) {
  list nghbrs;
  init_list(&nghbrs, sizeof(neighbour));

  neighbour n;
  init_neighbour(&n, 42, 42, 42*42, VOISIN_H, 42);
  list_add_front(&nghbrs, &n);
  CU_ASSERT(nghbrs.nb_elem == 1);
  CU_ASSERT(nghbrs.element_size == sizeof(neighbour));

  neighbour n2;
  list_get_index(&nghbrs, 0, &n2);
  CU_ASSERT(n.x == n2.x);
  CU_ASSERT(n.y == n2.y);
  CU_ASSERT(n.size == n2.size);
  CU_ASSERT(n.com_rank == n2.com_rank);
  CU_ASSERT(n.orientation == n2.orientation);
}

void test_LIST_REPLACE_INDEX(void) {
  list nghbrs;
  init_list(&nghbrs, sizeof(neighbour));

  neighbour n;
  init_neighbour(&n, 42, 42, 42*42, VOISIN_H, 42);
  list_add_front(&nghbrs, &n);
  CU_ASSERT(nghbrs.nb_elem == 1);
  CU_ASSERT(nghbrs.element_size == sizeof(neighbour));

  neighbour n2;
  init_neighbour(&n, 100, 100, 42*42, VOISIN_H, 52);
  list_add_front(&nghbrs, &n2);

  neighbour n3;
  list_get_index(&nghbrs, 1, &n3);
  CU_ASSERT(n3.x == n2.x);

  list_replace_index(&nghbrs, 1, &n);
  list_get_index(&nghbrs, 1, &n3);
  CU_ASSERT(n3.x == n.x);
}

void do_nothing (void * n) {UNUSED(n);}

void test_LIST_REMOVE_FRONT(void) {
  list nghbrs;
  neighbour n, n2, n3, out;
  init_neighbour(&n, 11, 11, 42*42, VOISIN_H, 42);
  init_neighbour(&n2, 22, 22, 42*42, VOISIN_H, 42);
  init_neighbour(&n3, 33, 33, 42*42, VOISIN_H, 42);

  init_list(&nghbrs, sizeof(neighbour));

  list_add_front(&nghbrs, &n);
  list_add_front(&nghbrs, &n2);
  list_add_front(&nghbrs, &n3);


  CU_ASSERT(nghbrs.nb_elem == 3);
  CU_ASSERT(list_get_index(&nghbrs, 0, &out));
  CU_ASSERT(out.x == 33);


  list_remove_front(&nghbrs, do_nothing);
  // [22, 33]
  CU_ASSERT(nghbrs.nb_elem == 2);
  CU_ASSERT(list_get_index(&nghbrs, 0, &out));
  CU_ASSERT(out.x == 22);

  list_remove_front(&nghbrs, do_nothing);
  // [33]
  CU_ASSERT(nghbrs.nb_elem == 1);
  CU_ASSERT(list_get_index(&nghbrs, 0, &out));
  CU_ASSERT(out.x == 11);

  list_remove_front(&nghbrs, do_nothing);
  // []
  CU_ASSERT(nghbrs.nb_elem == 0);
  CU_ASSERT(!list_get_index(&nghbrs, 0, &out));

  CU_ASSERT(!list_remove_front(&nghbrs, do_nothing));
}


void test_LIST_REMOVE_INDEX_simple(void) {
  list nghbrs;
  neighbour n, n2, n3, out;
  init_neighbour(&n, 11, 11, 42*42, VOISIN_H, 42);
  init_neighbour(&n2, 22, 22, 42*42, VOISIN_H, 42);
  init_neighbour(&n3, 33, 33, 42*42, VOISIN_H, 42);

  init_list(&nghbrs, sizeof(neighbour));

  list_add_front(&nghbrs, &n);
  // [11]

  list_add_front(&nghbrs, &n2);
  // [22, 11]

  list_add_front(&nghbrs, &n3);
  // [33, 22, 11]

  CU_ASSERT(nghbrs.nb_elem == 3);

  // clear out
  init_neighbour(&out, 0, 0, 0, VOISIN_H, 0);

  // après suppression du dernier
  CU_ASSERT(list_remove_index(&nghbrs, 0, do_nothing));
  CU_ASSERT(list_get_index(&nghbrs, 0, &out));
  // [22, 11]
  CU_ASSERT(nghbrs.nb_elem == 2);
  CU_ASSERT(out.x == 22);

  list_remove_index(&nghbrs, 1, do_nothing);

  // [22]
  CU_ASSERT(nghbrs.nb_elem == 1);
  CU_ASSERT(! list_get_index(&nghbrs, 1, &out));
  CU_ASSERT(list_get_index(&nghbrs, 0, &out));
  CU_ASSERT(out.x == 22);

  list_remove_index(&nghbrs, 0, do_nothing);
  // []
  CU_ASSERT(! list_get_index(&nghbrs, 1, &out));
  CU_ASSERT(nghbrs.nb_elem == 0);

  // []
  CU_ASSERT(! list_get_index(&nghbrs, 0, &out));
}

void test_LIST_REMOVE_INDEX(void) {
  list nghbrs;
  neighbour n, n2, n3, out;
  init_neighbour(&n,  11, 11, 42*42, VOISIN_H, 42);
  init_neighbour(&n2, 22, 22, 42*42, VOISIN_H, 42);
  init_neighbour(&n3, 33, 33, 42*42, VOISIN_H, 42);

  init_list(&nghbrs, sizeof(neighbour));

  list_add_front(&nghbrs, &n);
  // [11]

  list_add_front(&nghbrs, &n2);
  // [22, 11]

  list_add_front(&nghbrs, &n3);
  // [33, 22, 11]

  // après suppression de l'element au milieu
  list_remove_index(&nghbrs, 1, do_nothing);
  // [33, 11]
  CU_ASSERT(!list_get_index(&nghbrs, 2, &out));
  CU_ASSERT(nghbrs.nb_elem == 2);
  CU_ASSERT(list_get_index(&nghbrs, 1, &out));
  CU_ASSERT(out.x == n.x);
  CU_ASSERT(list_get_index(&nghbrs, 0, &out));
  CU_ASSERT(out.x == n3.x);

  list_remove_index(&nghbrs, 0, do_nothing);
  // [11]
  CU_ASSERT(nghbrs.nb_elem == 1);
  CU_ASSERT(! list_get_index(&nghbrs, 1, &out));
  CU_ASSERT(list_get_index(&nghbrs, 0, &out));
  CU_ASSERT(out.x == n.x);

  list_remove_index(&nghbrs, 0, do_nothing);
  // []
  // ya plus rien du tout
  CU_ASSERT(nghbrs.nb_elem == 0);
  CU_ASSERT(! list_get_index(&nghbrs, 0, &out));
}

void print_data_inner(const can_data * data) {
    printf("%d\n", *((int*)data->data));
}


int test (const void * c, const void *params) {
    chunk element = *(chunk *)c;
    pair pa = *(pair*) params;
    if (element.x == pa.x && element.y == pa.y) {
        return true;
    } else {
        return false;
    }
}

void test_LIST_FIND (void) {
    list l;
    chunk c_1, c_2, c_3, c_out;
    can_data data_1, data_2, data_3;
    pair p;
    int life_1 = 42;
    int life_2 = 42 * 42;
    int life_3 = 42 * 42 * 42;
    init_pair(&p, 45 ,46);
    init_data(&data_1, sizeof(int), DATA_INT, &life_1);
    init_data(&data_2, sizeof(int), DATA_INT, &life_2);
    init_data(&data_3, sizeof(int), DATA_INT, &life_3);

    init_chunk(&c_1, 43, 44, &data_1);
    init_chunk(&c_2, 45, 46, &data_2);
    init_chunk(&c_3, 47, 48, &data_3);

    init_list(&l, sizeof(chunk));

    list_add_front(&l, &c_1);
    list_add_front(&l, &c_2);
    list_add_front(&l, &c_3);

    list_find(&l,&p ,test, &c_out);

}

/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int main()
{
   CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite("Suite_1", init_suite1, clean_suite1);
   if (NULL == pSuite) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   if (

     (NULL == CU_add_test(pSuite, "test of 42 !!", test_42)) ||
     (NULL == CU_add_test(pSuite, "test of init_list()", test_INIT_LIST)) ||
     (NULL == CU_add_test(pSuite, "test of list_add_front()", test_LIST_ADD_FRONT)) ||
     (NULL == CU_add_test(pSuite, "test of list_replace_index()", test_LIST_REPLACE_INDEX)) ||
     (NULL == CU_add_test(pSuite, "test of list_remove_front()", test_LIST_REMOVE_FRONT)) ||
     (NULL == CU_add_test(pSuite, "test of list_remove_index() version moins simple", test_LIST_REMOVE_INDEX)) ||
     (NULL == CU_add_test(pSuite, "test of list_remove_index() version simple", test_LIST_REMOVE_INDEX_simple)) ||
     (NULL == CU_add_test(pSuite, "test of list_find", test_LIST_FIND))

     ) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* Run all tests using the CUnit Basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}
