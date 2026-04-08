#include <QSyntaxHighlighter>
#include <string>
#include <unordered_map>
#include <vector>
extern "C" {
struct TSTree;
struct TSParser;
struct TSQuery;
struct TSQueryCursor;
struct TSNode;
}
class YoyoHighlighter : public QSyntaxHighlighter {
public:
    TSTree* tree = nullptr;
    TSParser* parser = nullptr;
    std::vector<TSQuery*> highlightQueries;
    TSQueryCursor* queryCursor;
    struct HighlightObj {
        uint32_t start_byte, end_byte;
        std::string capture_name;
    };
    std::vector<HighlightObj> highlights;
    std::unordered_map<std::string, QTextCharFormat> hl_theme;
    explicit YoyoHighlighter(QObject* parent);
    void highlightBlock(const QString& text) override;
    void refreshHighlights(int position, int removed, int added);
    void changeDocument(QTextDocument* doc);
    void initCatppuccin();
    void initialHighlight();
    void buildQueries();
    void runQueries(const TSNode &);
    ~YoyoHighlighter();
};
