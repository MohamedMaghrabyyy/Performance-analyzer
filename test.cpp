

void a();
void b();
void c();
void d();
void e();

void a() {
    
    for (int i = 0; i < 100000; i++) {
        // Simulate work
    }
    b();
 
}

void b() {
    
    for (int i = 0; i < 200000; i++) {
        // Simulate work
    }
    c();
    
}

void c() {
 
    for (int i = 0; i < 400000; i++) {
        // Simulate work
    }
    d();
   
}

void d() {
   
    for (int i = 0; i < 800000; i++) {
        // Simulate work
    }
    e();
    
}

void e() {
 
    for (int i = 0; i < 1600000; i++) {
        // Simulate work
    }
    
}

int main() {
    
    a();
    for (int i = 0; i < 1600000; i++) {
        // Simulate work
    }
    
    b();
    
}
