#include <iostream>
#include <cstring>  // For strcpy and new
#include <vector>

#define TEXT       0x01
#define DATE       0x02
#define INTEGER    0x04
#define PRIMARY_KEY 0x10
#define FOREIGN_KEY 0x20

// strong typing allows for compile time errors when foreign key or other restrictions are violated
// we can basically add static_asserts that would occur during compile time via templating for this

// Macro to define a field and automatically initialize the field's type and name
#define Field(name, type) FieldType name(type, #name)

// Base structure for FieldType
struct FieldType 
{
    const char* string;  // Pointer to a constant field name string
    unsigned int value;  // Field type and status

    // Constructor to initialize value and string directly
    FieldType(unsigned int value, const char* fieldName) 
        : string(fieldName), value(value) {}


    // Convert the field type (int) to a human-readable string
    std::string toString() const 
    {
        std::string result(string + " ");
        if (value & TEXT) result += "TEXT ";
        if (value & DATE) result += "DATE ";
        if (value & INTEGER) result += "INTEGER ";
        if (value & PRIMARY_KEY) result += "PRIMARY KEY ";
        if (value & FOREIGN_KEY) result += "FOREIGN KEY ";
        return result;
    }
};


#define TABLE(name, f, ...) \
    struct name { \
        static const char* string = #name; \
        static const FieldType* fields = f; /* array of FieldType */ \
        __VA_ARGS__ \
        /* Insert any additional static fields specified by the user */ \
    };


// Table class template to instantiate and handle the SQL-like table operations
template <typename T>
class Table 
{
    public:
    // Constructor to instantiate the table (could initialize some data here)
    Table() 
    {
        std::cout << "Table for struct " << T.name << " created!" << std::endl;
        // setup SQLite in memory database/initialize subsystem if not already started (simple check)
        // create table T in the in memory SQLite database
    }

    void SQL(const char* SQL)
    {
        // runs SQL query against SQLite heap representation
        return;
    }

    // Print the table's field names and types
    void printFields() const 
    {
        std::cout << "Fields for table " << T::string << ":" << std::endl;
        std::vector<std::string> fields;
        getFieldNamesAndTypes(fields);
        for (std::string i : fields)
            std::cout << "\t" << i << std::endl;
    }
    
    // Fill a vector with formatted field names and types (SQLite compatible format)
    void getFieldNamesAndTypes(std::vector<std::string>& result) const 
    {
        for (size_t i = 0; i < sizeof(T::fields) / sizeof(FieldType); ++i) 
            result.push_back(T::fields[i].toString());
    }
    
};


/*
// Example table definition using the TABLE macro
TABLE(Movie,
    [Field(title, TEXT),
    Field(director, TEXT), 
    Field(producer, TEXT),
    Field(releaseDate, DATE),
    Field(movieId, INTEGER | PRIMARY_KEY)]
    static int customMetadataField;  // Custom static field
);

int main() {
    Movie movie; // now this metadata struct exists
    
    Table<Movie> movieTable; // and you can instantiate an in-heap representation like this
    // Print the field names
    movieTable.printFields();
    for (FieldType* field : movie.fields) std::cout << field.toString() << std::endl;

    return 0;
}
*/
