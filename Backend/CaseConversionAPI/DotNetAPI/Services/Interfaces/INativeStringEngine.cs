namespace StringConversionAPI.Services
{
    public interface INativeStringEngine
    {
        string Name { get; }
        string Convert(string input, int choice);
        Task<IEnumerable<string>> ConvertBatchAsync(IEnumerable<string> texts, int choice);
    }
}