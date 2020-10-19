#include <node.h>

#include <cstring>
#include <termios.h>
#include <unistd.h>

#if __APPLE__
#include <util.h>
#endif

#if __linux__
#include <pty.h>
#endif

static void spawn(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    int master_fd;
    int pid = forkpty(&master_fd, NULL, NULL, NULL);

    if (pid == 0) {
        execl("/bin/sh", "sh", NULL);
    }

    args.GetReturnValue().Set(v8::Number::New(args.GetIsolate(), master_fd));
}

static void initialize(v8::Local<v8::Object> exports)
{
    NODE_SET_METHOD(exports, "spawn", spawn);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, initialize)
