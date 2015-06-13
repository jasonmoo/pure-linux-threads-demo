package main

import (
	"os"
	"runtime"
	"sync"
	"sync/atomic"
)

func main() {
	runtime.GOMAXPROCS(runtime.NumCPU())

	var (
		runs  int64 = 100 << 20
		hello       = []byte(`Hello from \e[91;1mthread\e[0m!\n\0`)
		wg          = sync.WaitGroup{}
	)

	wg.Add(runtime.NumCPU()+1)

	for i := 0; i < runtime.NumCPU()+1; i++ {
		go func() {
			defer wg.Done()
			for {
				os.Stdout.Write(hello)
				if atomic.AddInt64(&runs, -1) < 0 {
					return
				}
			}
		}()
	}

	wg.Wait()

}

