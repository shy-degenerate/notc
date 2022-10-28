#ifndef _NOTC_H_
#define _NOTC_H_

class Notc {

	public:
		static bool hadError;

	public: 	
		static int exec(const std::string& source);

		static int run_from_file(const std::string& file_path);

		static void run_prompt();

		static void message(int line, const std::string msg);
};

#endif // _NOTC_H_
