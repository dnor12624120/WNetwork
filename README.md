# WNetwork
A C++ wrapper library for TCP/UDP sockets
## 9/14/2019 changes
- adopted a namespace (`WNetwork`/`Types` for types)
- renamed `SendAll`/`ReceiveAll` to `Send`/`Receive`
- renamed `Send`/`Receive` to `SendImplementation`/`ReceiveImplementation`
- added `ReceiveAll` which reads an indeterminate amount of data (size upated through reference size argument)
- replaced macros with constants
- replaced `typedef`s with `using` statements
- removed overly verbose comments (general documentation still pending)

