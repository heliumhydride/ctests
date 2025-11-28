#define _XOPEN_SOURCE   600
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdint.h>
#include <assert.h>

class node {
  public:
    int data;
    node* next;

    node(int value, node* n) {
      data = value;
      next = n;
    }

    void affiche(void) {
      printf("val=%d, ptr=", this->data);
      if(this->next == NULL)
        printf("null\n");
      else
        printf("0x%zx\n", (intptr_t)this->next);
    }
};

class list {
  private:
    node* head;

  public:
    list(node* h) {
      this->head = h;
    }
 
    size_t length(void) {
      size_t count = 1;
      node* tmpnode = this->head;

      if(tmpnode == NULL)
        return 0;

      while(tmpnode->next != NULL) {
        tmpnode = tmpnode->next;
        count++;
      }
      return count;
    }

    int operator[](size_t pos) {
      assert(pos < this->length() && "List index out of range");
      node* tmpnode = this->head;
      for(size_t i = 0; i < pos; i++)
        tmpnode = tmpnode->next;
      return tmpnode->data;
    }

    void affiche(void) {
      printf("{");
      for(size_t i = 0; i < this->length(); i++) {
        printf("%d", (*this)[i]);
        if(i < (this->length() - 1))
          printf(", "); // if we are not at the end of list, add a separator
      }
      printf("}\n");
    }

    void insert(int x, size_t pos) {
      if(pos == 0) {
        node tmpnode(x, this->head);
        this->head = &tmpnode;
      }
    }

    void insert(int x) { // overloaded version with no pos parameters, defaults to inserting at the beginning
      this->insert(x, 0);
    }

    void append(int x) {
      this->insert(x, this->length());
    }

    node debug_head(void) {
      return *(this->head);
    }

};

int main(void) {
  node c(3,NULL);
  node b(2,&c);
  node a(1,&b);
  a.affiche(); b.affiche(); c.affiche();
  list L(&a);
  L.affiche();
  
  list empty_list(NULL);
  empty_list.affiche();
  /*
  for(int x = 0; x <= 9; x++)
    empty_list.insert(x);
  */
  empty_list.insert(1);
  //empty_list.affiche();
  printf("%d", empty_list[0]);
  empty_list.debug_head().affiche();
  return 0;
}
