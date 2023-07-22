#include <iostream>
#include <fstream>
#include <string>

bool does_file_exist( const std::string& filename ) {
	std::ifstream file ( filename );

	return file.is_open( );
}

void file_not_found( const std::string& filename ) {
	std::cout << "File \"" << filename << "\" not found." << std::endl;
}

void view_todos( const std::string& filename ) {
	// Open the file, if it doesn't exist then it creates it
	std::ifstream todo_file ( filename );

	if ( todo_file.is_open( ) ) {
		// Check if the file is empty
		if ( todo_file.peek( ) == std::ifstream::traits_type::eof( ) ) {
			std::cout << "No to-do items." << std::endl;
		} else {
			int line_count { 1 };
			std::string line;
			// First is reference to the file, second is the variable reference to store the line
			while ( getline( todo_file, line ) ) {
				std::cout << line_count << ": " << line << std::endl; // Print out every to-do item
				line_count++;
			}

			todo_file.close( );
		}
	} else {
		file_not_found( filename );
	}

	std::cout << std::endl;
}

void add_todo( const std::string& filename, const std::string& todo_item ) {
	// Read the data from the file and store it in a string
	std::string file_data;

	std::ifstream todo_file ( filename );

	if ( todo_file.is_open( ) ) {
		std::string line;
		while ( getline( todo_file, line ) ) {
			file_data += line + "\n";
		}

		todo_file.close( );
	} else {
		file_not_found( filename );
		return;
	}

	todo_file.close( );

	// Open the file
	std::ofstream todo_file_write ( filename );

	if ( todo_file_write.is_open( ) ) {
		todo_file_write << file_data; // Write old data
		todo_file_write << todo_item << std::endl; // Write added item
		todo_file_write.close( );
	} else {
		file_not_found( filename );
	}
}

void remove_todo( const std::string& filename, const std::string& item_to_remove ) {
	// Open the file in read mode
	std::ifstream todo_file ( filename );

	if ( todo_file.is_open( ) ) {
		// Check if item_to_remove is numeric
		bool is_numeric { item_to_remove[ 0 ] == '#' };

		int numeric_item_to_remove { 0 };

		if ( is_numeric ) {
			numeric_item_to_remove = std::stoi( item_to_remove.substr( 1, item_to_remove.size( ) - 1 ) );
		}

		std::string line;
		std::string file_data;

		int line_num { 1 };
		bool already_found { false };

		while ( getline( todo_file, line ) ) {
			if ( ( !is_numeric && line.find( item_to_remove ) == std::string::npos ) || ( is_numeric && ( numeric_item_to_remove != line_num ) ) || already_found  ) {
				file_data += line + "\n";
			} else {
				std::cout << "Removed \"" << line << "\"." << std::endl;
				already_found = true;
			}

			line_num++;
		}

		file_data = file_data.substr( 0, file_data.size( ) - 1 ); // Remove the last newline character

		todo_file.close( );

		std::ofstream todo_file_write ( filename );

		if ( todo_file_write.is_open( ) ) {
			todo_file_write << file_data;
		}

		todo_file_write.close( );
	} else {
		file_not_found( filename );
	}
}

void mark_todo_as_done( const std::string& filename ) {
	int line_number;
	std::cout << "Enter the number of the to-do item: ";
	std::cin >> line_number;

	bool todo_already_done { false };

	// Check if item is already marked as todo_already_done
	// Open the file in read mode
	std::ifstream todo_file ( filename );

	std::string before { };
	std::string after { };
	std::string modified_line { };

	if ( todo_file.is_open( ) ) {
		std::string line;
		int line_num { 1 };

		while ( getline( todo_file, line ) ) {
			if ( line_num == line_number ) {
				// Check if line starts with "todo_already_done -"
				if ( line.find( "done -" ) == 0 ) {
					std::cout << "To-do item is already marked as done." << std::endl;
					modified_line = line;
				} else {
					modified_line = "done - " + line;
					todo_already_done = true;
				}
			} else if ( line_num < line_number ) {
				before += line + "\n";
			} else if ( line_num > line_number ) {
				after += line + "\n";
			}

			line_num++;
		}

		after = after.substr( 0, after.length( ) - 1 );
	} else {
		file_not_found( filename );
	}

	todo_file.close( );

	if ( todo_already_done ) {
		std::cout << "To-do #" << line_number << " is already marked as done." << std::endl;
	}

	std::ofstream todo_file_write ( filename );

	if ( todo_file_write.is_open( ) ) {
		todo_file_write << before;
		todo_file_write << modified_line << std::endl;
		todo_file_write << after;
	} else {
		file_not_found( filename );
	}
}

void output_options_list( ) {
	std::cout << "What would you like to do?" << std::endl;
	std::cout << "1) Add a to-do" << std::endl;
	std::cout << "2) Remove a to-do" << std::endl;
	std::cout << "3) Mark a todo as done" << std::endl;
	std::cout << "4) View to-dos" << std::endl;
	std::cout << "5) Quit" << std::endl << std::endl;
	std::cout << "Your choice: ";
}

int main( ) {
	std::cout << "[project] To-do list." << std::endl;

	std::string filename { "todo.txt" };

	if ( !does_file_exist( filename ) ) {
		std::cout << "File \"" << filename << "\" not found, creating it." << std::endl;
		std::ofstream todo_file ( filename );
		todo_file.close( );
	}

	view_todos( filename );

	char choice;
	std::string todo_item;

	while( true ) {
		output_options_list( );

		std::cin >> choice;

		if ( choice == '5' ) {
			break;
		} else if ( choice == '1' || choice == '2' ) {
			std::cout << "Enter a to-do item: ";

			// Read everything, even the spaces
			std::cin.ignore( std::numeric_limits<std::streamsize>::max( ), '\n' );
			std::getline( std::cin, todo_item );

			if ( choice == '1' ) {
				add_todo( filename, todo_item );
			} else {
				remove_todo( filename, todo_item );
			}
		} else if ( choice == '3' ) {
			mark_todo_as_done( filename );
		} else if ( choice == '4' ) {
			view_todos( filename );
		} else {
			std::cout << "Invalid choice" << std::endl;
		}
	}

	std::cout << "Exiting..." << std::endl;

	return 0;
}
