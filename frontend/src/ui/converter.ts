/**
 * @file        converter.ts
 * @summary     UI Controller for the String Conversion Interface.
 * @description Orchestrates DOM event handling and state synchronization 
 * between the HTML View and the WordCase API Service.
 * * @copyright   (c) 2016–2026 nitishhsinghh. All rights reserved.
 * Educational use only.
 * * @version     1.1.1
 * @author       Nitish Singh
 * @lastMachine 2026-05-06
 */

import { convertText } from "../api/wordCaseApi";
import { ConvertRequest, ConvertResponse } from "../api/wordCaseApi";

/**
 * Event Handler for the 'Convert' action. 
 * Extracts data from the DOM, invokes the native conversion pipeline via REST,
 * and updates the UI with the result.
 * * @async
 * @function onConvertClick
 * @returns {Promise<void>}
 */
async function onConvertClick(): Promise<void> {
    // 1. Acquire DOM References with Non-Null Assertions
    const inputElement = document.getElementById("inputText") as HTMLInputElement;
    const choiceElement = document.getElementById("choice") as HTMLSelectElement;
    const outputElement = document.getElementById("output");

    if (!inputElement || !choiceElement || !outputElement) {
        console.error("[UI] Critical Error: Required DOM elements are missing.");
        return;
    }

    // 2. Prepare Request Payload
    const request: ConvertRequest = {
        text: inputElement.value,
        choice: parseInt(choiceElement.value, 10)
    };

    try {
        // 3. Set UI to 'Loading' state (Principal Engineering Best Practice)
        outputElement.textContent = "Processing...";

        // 4. Dispatch to API Service
        const result: ConvertResponse = await convertText(request);

        // 5. Update View with Native Result
        // CHANGE: Access 'convertedText' instead of 'output'
        outputElement.textContent = result.convertedText;

    } catch (error) {
        // 6. Error Boundary Handling
        const errorMessage = error instanceof Error ? error.message : "Unknown Error";
        outputElement.textContent = `Error: ${errorMessage}`;
        console.error(`[API] Execution Failed: ${errorMessage}`);
    }
}

/**
 * Global Export
 * Attaches the handler to the window object to facilitate invocation from 
 * vanilla HTML 'onclick' attributes.
 */
(window as any).onConvertClick = onConvertClick;