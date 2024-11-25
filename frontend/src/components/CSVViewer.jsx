import React, { useEffect, useState } from 'react';
import axios from 'axios';
import Papa from 'papaparse';
import './CSVViewer.css'; // Import the CSS file

const CSVViewer = () => {
    const [csvData, setCsvData] = useState([]);
    const [error, setError] = useState('');

    useEffect(() => {
        const fetchCSV = async () => {
            try {
                const response = await axios.get('http://127.0.0.1:8000/get-response-csv/', {
                    headers: {
                        'Content-Type': 'text/csv',
                    },
                });
                
                // Parse the CSV data
                Papa.parse(response.data, {
                    header: true,
                    complete: (result) => {
                        setCsvData(result.data);
                    },
                    error: (error) => {
                        console.error('Error parsing CSV:', error);
                        setError('Error parsing the CSV file.');
                    },
                });
            } catch (error) {
                console.error('Error fetching CSV:', error);
                setError(
                    error.response && error.response.data.error 
                        ? error.response.data.error 
                        : 'Failed to fetch the CSV file. Please try again.'
                );
            }
        };

        fetchCSV();
    }, []);

    return (
        <div className="csv-viewer">
            <h2>Execution Report</h2>
            {error && <p className="error-message">{error}</p>}
            {!error && csvData.length > 0 ? (
                <table className="csv-table">
                    <thead>
                        <tr>
                            {Object.keys(csvData[0]).map((header, index) => (
                                <th key={index}>{header}</th>
                            ))}
                        </tr>
                    </thead>
                    <tbody>
                        {csvData.map((row, rowIndex) => (
                            <tr key={rowIndex}>
                                {Object.values(row).map((cell, cellIndex) => (
                                    <td key={cellIndex}>{cell}</td>
                                ))}
                            </tr>
                        ))}
                    </tbody>
                </table>
            ) : (
                !error && <p>No data available.</p>
            )}
        </div>
    );
};

export default CSVViewer;