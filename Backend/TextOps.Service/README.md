# Data Ingestion & Dictionary Management

The platform supports adding new words to the native Lexis.Core dictionary through the TextOps.Api gateway. These words are persisted to the native dictionary store using zero-copy memory bridging.

## Dictionary Initialization and Lifecycle

This project uses a multi-stage pipeline to transform raw frequency data into a high-performance native dictionary. We use the path src/Lexis.Core/data/dictionary.txt as the persistent backing store for the Lexis.Core engine.

### 1. Data Acquisition (External to Local)

First, the raw 100k Wiktionary dataset is pulled into the testing environment.

```Bash
curl -L -o vocabulary/dictionary.txt https://gist.githubusercontent.com/h3xx/1976236/raw/wiki-100k.txt
```

### Native Persistence (API to Engine Store)

Once the API receives the stream, the Lexis.Core engine indexes the words in memory and flushes them to the permanent dictionary file.

- Target File: src/Lexis.Core/data/dictionary.txt

- Method: SpellChecker::Insert() handles the write-through logic.

- Result: The dictionary becomes "warmed," allowing for spell-checks and suggestions.
