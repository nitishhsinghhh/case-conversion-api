/**************************************************************************************************
 * File         : ConvertResponse.cs
 *
 * Copyright    : (c) 2016–2026 nitishhsinghh. All rights reserved.
 * This material may be reproduced for teaching and learning purposes only.
 * It is not to be used in industry or for commercial purposes.
 *
 * Class        : ConvertResponse
 *
 * Description  : Data transfer object (DTO) representing the response payload for
 * string conversion operations. Encapsulates the original input,
 * selected conversion strategy, and processed output string.
 *
 * Notes        : - Returned by WordCaseController endpoints.
 * - Defines the API response contract for client consumption.
 *
 * Revision History:
 * ------------------------------------------------------------------------------------------------
 * Version     Date           Author           Description
 * ------------------------------------------------------------------------------------------------
 * 1.0         2026-04-11     Nitish Singh     Initial implementation of response model.
 * 1.1         2026-04-20     Nitish Singh     Added ConvertedText property to resolve CS0117.
 * Mapped Output as a calculated property for 
 * backward compatibility.
 **************************************************************************************************/

using System.Text.Json.Serialization;
public class ConvertResponse
{
    [JsonPropertyName("input")]
    public string Input { get; set; } = string.Empty;
    
    [JsonPropertyName("choiceId")] 
    public int Choice { get; set; }

    /// <summary>
    /// The final processed string from the native C++ engine.
    /// </summary>
    [JsonPropertyName("convertedText")] // Consistent with legacy Output
    public string ConvertedText { get; set; } = string.Empty;

    /// <summary>
    /// Maps to ConvertedText for legacy compatibility.
    /// </summary>
    [JsonIgnore] // Use JsonIgnore so it doesn't collide or bloat the payload
    public string Output 
    { 
        get => ConvertedText; 
        set => ConvertedText = value; 
    }
}