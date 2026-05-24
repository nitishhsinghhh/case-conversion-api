/**
 * @file        wordCaseApi.ts
 * @summary     Frontend Service Layer for the Word Case Conversion API.
 * @description Provides a type-safe interface for communicating with the 
 * .NET/C++ backend engine. Handles request serialization, 
 * transport via Fetch API, and response deserialization.
 * @copyright   (c) 2016–2026 nitishhsinghh. All rights reserved.
 * @version     1.1.0
 * @author      Nitish Singh
 */

/**
 * Represents the structured request for the conversion engine.
 */
export interface ConvertRequest {
  /** The raw text to be processed. */
  text: string;
  /** The integer ID corresponding to a specific C++ strategy. */
  choice: number;
}

/**
 * Represents the normalized response from the API Gateway.
 * Synchronized with backend JSON keys: input, choiceId, convertedText.
 */
export interface ConvertResponse {
  /** The original input text. */
  input: string;
  /** The strategy ID utilized for processing. */
  choiceId: number;
  /** The resulting string after native transformation. */
  convertedText: string;
}

/**
 * Dispatches a transformation request to the Backend API.
 * 
 * @async
 * @param {ConvertRequest} request - The payload containing text and strategy ID.
 * @returns {Promise<ConvertResponse>} A promise resolving to the processed result.
 * @throws {Error} If the network request fails or returns a non-200 status.
 * 
 * @example
 * const result = await convertText({ text: "hello", choice: 4 });
 * console.log(result.convertedText); // "HELLO"
 */
export async function convertText(
  request: ConvertRequest
): Promise<ConvertResponse> {
  const API_ENDPOINT = "http://localhost:5050/api/WordCase/convert";

  // Logical check: choice can be 0, so we check against undefined
  if (!request.text || request.choice === undefined) {
    throw new Error("Client Error: Text and Choice are required for conversion.");
  }

  try {   
    const response = await fetch(API_ENDPOINT, {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
        "Accept": "application/json"
      },
      body: JSON.stringify(request)
    });

    if (!response.ok) {
      const errorDetail = await response.text(); 
      throw new Error(`API Error: ${response.status} - ${errorDetail || response.statusText}`);
    }

    // Returns the JSON mapped to the ConvertResponse interface
    return await response.json();

  } catch (error) {
    console.error("[wordCaseApi] Transport Failure:", error);
    throw error;
  }
}