link merge (link a, link  n)
{
    struct node head; link c = &head;
    while (a && b)
        if(a->item < b->item){
            c->next = a;
            c=a;
            a=a->next;
        }
        else{
            c->next=b;
            c=b;
            b=b->next;
        }
    c->next = !a ? b : a;
    return head.next;
}

link lmergesort(link c)
{
    link a, b;
    if(!c->next) return c;
    a = c;
    b = c->next;
    while(b && b->next){
        c = c->next; b=b->next->next;
    }
    b = c-> next;
    c->next = NULL;
    return merge(lmergesort(a), lmergesorte(b));
}

