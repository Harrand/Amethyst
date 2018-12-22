namespace am
{
    inline void initialise()
    {
#ifdef AMETHYST_WINDOWS
        windows::initialise();
#elif AMETHYST_UNIX
        unix::initialise();
#endif
        am::detail::is_initialised = true;
    }

    inline void terminate()
    {
#ifdef AMETHYST_WINDOWS
        windows::terminate();
#elif AMETHYST_UNIX
        unix::terminate();
#endif
        am::detail::is_initialised = false;
    }

#ifdef AMETHYST_WINDOWS
    namespace windows
        {
           inline void initialise()
           {
               am::debug::print("Windows am::initialise() invoked.\n");
               WSADATA wsadata;
               WSAStartup(MAKEWORD(2,2), &wsadata);
           }

          inline void terminate()
          {
              am::debug::print("Windows am::terminate invoked.\n");
              WSACleanup();
          }
        }
#elif AMETHYST_UNIX
    namespace unix
        {
            inline void initialise()
            {
               am::debug::print("Unix am::initialise invoked.\n");
            }

            inline void terminate()
            {
                am::debug::print("Unix am::terminate invoked.\n");
            }
        }
#endif
}