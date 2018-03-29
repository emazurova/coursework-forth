: string-concat dup
count
rot
dup
count
rot
over
+
1
+
heap-alloc
rot
swap
dup
rot
string-copy
dup
rot
+
rot
string-copy ;
