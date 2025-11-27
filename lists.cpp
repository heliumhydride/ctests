#define _XOPEN_SOURCE   600
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdint.h>

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
      size_t count;
      node* tmpnode = this->head;
      while(tmpnode.next == NULL)
        count++;
      return count;
    }

    int operator[](size_t pos) {
      assert(pos < this->length() && "List index out of range");
      node* tmpnode = this->head;
      for(size_t i = 0; i < this->length(); i++)
        tmpnode = tmpnode.next;
      return tmpnode.data;
    }

    void affiche(void) {
      printf("{");
      for(size_t i = 0; i < this->length(); i++) {
        printf("%d", *this[i]);
      }
      printf("}\n");
    }
};

int main(void) {
  node c(3,NULL);
  node b(2,&c);
  node a(1,&a);
  a.affiche(); b.affiche(); c.affiche();
  list L(a);
  L.affiche();
  return 0;
}
