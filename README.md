<a name="readme-top"></a>

<!-- ABOUT THE PROJECT -->
## Mini Shell

This can be used as the most minimal UI for your project. Since it's not dependent on a specific interface type, this can be used with various interfaces, including stdin/out as well as UART, Ethernet, and more.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- GETTING STARTED -->
## Getting Started
The following code is an example of running a shell using stdin/out.

```
#include <stdio.h>
#include <stdint.h>
#include "shell.h"

#define MAX_INPUT_SIZE (1024)

int32_t s_shell_out(uint8_t* p_bytes, uint32_t size)
{
    snprintf(stdout, size, "%s", p_bytes);
    return 0;
}

int main(void) 
{
    uint8_t input_buffer[MAX_INPUT_SIZE];
    SH_CONFIG _config = {
        .name = "shell",
        .init = NULL,
        .output =  s_shell_out
    };

    sh_init(&_config);

    while (1) {
		if (fgets((char*)input_buffer, MAX_INPUT_SIZE, stdin) != NULL) {
			size_t input_length = strlen((char*)input_buffer);
			sh_input(input_buffer, input_length);
		}
    }
    return 0;
}
```

<!-- ROADMAP -->
## Roadmap

- [x] Add basic usages
- [ ] Add Windows(stdio), Linux(stdio), STM32(uart) examples
   
<p align="right">(<a href="#readme-top">back to top</a>)</p>
