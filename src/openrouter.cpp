#include "OpenRouter/OpenRouter.hpp"

int main()
{
    OpenRouter router;
    return router.serve("127.0.0.1", 43244);
}
